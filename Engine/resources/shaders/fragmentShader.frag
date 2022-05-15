#version 450 core

in vec3 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec2 tilling;
uniform float farPlane;

out vec4 FragColor;

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
	float shininess;

	sampler2D alphaTexture;
	sampler2D ambientTexture;
	sampler2D diffuseTexture;
	sampler2D emissiveTexture;
	sampler2D specularTexture;
};

uniform Material material;

struct Light
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 attenuation;
	float spotCutoff;

	vec3 spotDirection;
	float spotOuterCutoff;

	bool enable;
	bool hasShadow;
	
	mat4 spaceMatrix;
};

#define NBR_LIGHTS 8
Light lights[NBR_LIGHTS];

uniform mat4 lightAttribs2[NBR_LIGHTS][1];
uniform mat4 lightAttribs1[NBR_LIGHTS][1];
uniform mat4 lightAttribs3[NBR_LIGHTS][1];
uniform sampler2D shadowMaps[NBR_LIGHTS][1];
uniform samplerCube shadowCubeMaps[NBR_LIGHTS][1];

void parseLights()
{
	// Parse each light matrix to a light struct
	for (int i = 0; i < NBR_LIGHTS; i++)
	{
		lights[i].position	= lightAttribs1[i][0][0];
		lights[i].ambient	= lightAttribs1[i][0][1];
		lights[i].diffuse	= lightAttribs1[i][0][2];
		lights[i].specular	= lightAttribs1[i][0][3];

		lights[i].attenuation		= vec3(lightAttribs2[i][0][0]);
		lights[i].spotCutoff		= cos(lightAttribs2[i][0][0][3]);
		lights[i].spotDirection		= vec3(lightAttribs2[i][0][1]);
		lights[i].spotOuterCutoff	= cos(lightAttribs2[i][0][1][3]);
		lights[i].enable			= bool(lightAttribs2[i][0][2][0]);
		lights[i].hasShadow			= bool(lightAttribs2[i][0][2][1]);

		lights[i].spaceMatrix = lightAttribs3[i][0];
	}
}

float getDirectionalShadow(int indexLight)
{
	float shadow = 0.0;

	// Perspcetive divide
	vec4 fragPosLightSpace = lights[indexLight].spaceMatrix * vec4(FragPos, 1.0);
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// [0,1]
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMaps[indexLight][0], projCoords.xy).r;
	float currentDepth = projCoords.z;
	vec3 normal = normalize(Normal);

	vec3 lightDir = normalize(lights[indexLight].position.xyz - FragPos);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	// Apply Percentage-Closer filtering to avoid "stair" shadows
	// Use to soft shadow boders

	// Avoid shadow out of the frustum
	if (projCoords.z > 1.0)
		return 0.0;

	// Calculate the texel size from the depth texture size
	vec2 texelSize = 1.0 / textureSize(shadowMaps[indexLight][0], 0);

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(shadowMaps[indexLight][0], projCoords.xy + vec2(x, y) * texelSize).r;

			// Compare pcf and current depth of fragment to determine shadow
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	return shadow / 9;
}

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float getPointShadow(int indexLight)
{
	vec3 fragToLight = FragPos - lights[indexLight].position.xyz;
	float currentDepth = length(fragToLight);

	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - FragPos);
	float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;
	for (int i = 0; i < samples; i++)
	{
		float closestDepth = texture(shadowCubeMaps[indexLight][0], fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= farPlane;
		if (currentDepth - bias > closestDepth)
			shadow += 1.0;
	}

	shadow /= float(samples);

	return shadow;
}

float getShadow(int indexLight)
{
	return getDirectionalShadow(indexLight);
	/*if (lights[indexLight].position.w == 0.0)
		return getDirectionalShadow(indexLight);

	return getPointShadow(indexLight);*/
}

void getLightColor(out vec4 ambient, out vec4 diffuse, out vec4 specular, out float shadow)
{
	ambient = diffuse = specular = vec4(0.0, 0.0, 0.0, 1.0);

	shadow = 0.0;

	// Initialize some variables to avoid to calculate them another time
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	for (int i = 0; i < NBR_LIGHTS; i++)
	{
		if (!lights[i].enable)
			continue;

		if (lights[i].hasShadow)
		{
			shadow += getShadow(i);

			if (shadow > 1.0)
				shadow = 1.0;
		}

		// Get light direction, if the light is a point light or a directionnal light
		vec3 lightDir = lights[i].position.xyz - FragPos * lights[i].position.w;
	
		// Compute the light direction and the distance between the fragment and the light
		float distance = length(lightDir);

		// Normalize the light direction manually
		lightDir /= distance;

		float finalIntensity = 1.0;

		// If the light is not a directionnal light, compute the final intensity
		if (lights[i].position.w != 0.0)
		{
			// Get spot cutoff and spot intensity 
			float theta = dot(lightDir, normalize(lights[i].spotDirection));
			float spotIntensity = clamp((theta - lights[i].spotOuterCutoff) / (lights[i].spotCutoff - lights[i].spotOuterCutoff), 0.0, 1.0);

			// Get attenuation (c + l * d + q * d≤)
			float attenuation = lights[i].attenuation.x +
								lights[i].attenuation.y * distance +
								lights[i].attenuation.z * distance * distance;

			// Apply attenuation  to the spot intensity
			finalIntensity = spotIntensity / attenuation;
		}

		// Pre-compute normal ÅElightDir
		float NdotL = dot(normal, lightDir);

		// Compute ambient
		ambient += lights[i].ambient;

		// Compute diffuse
		diffuse += max(NdotL, 0.0) * finalIntensity * lights[i].diffuse;

		// Compute specular
		vec3 reflectDir = 2.0 * NdotL * normal - lightDir;  
		specular += pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * finalIntensity * lights[i].specular;
	}

	shadow = 1.0 - shadow;
}

vec2 getTilledTexCoords()
{	
	return TexCoord.st * tilling.x + tilling.y;
}

void main()
{
	parseLights();

	vec4 ambient, diffuse, specular;
	float shadow;

	getLightColor(ambient, diffuse, specular, shadow);

	vec2 tilledTexCoords = getTilledTexCoords();

	vec4 diffColor = texture(material.diffuseTexture, tilledTexCoords);

	if (diffColor.a < 0.1)
		discard;

	vec4 ambientColor = ambient * (material.ambient + texture(material.ambientTexture, tilledTexCoords));

	vec4 diffuseColor = material.diffuse * diffuse * shadow;

	vec4 specularColor = specular * shadow * (material.specular + texture(material.specularTexture, tilledTexCoords));

	vec4 emissiveColor = material.emissive + texture(material.emissiveTexture, tilledTexCoords);

	// Get texture color applied to the light
	vec4 shadedColor = (ambientColor + diffuseColor) * diffColor + emissiveColor + specularColor;
	FragColor = shadedColor;

	FragColor.a = texture(material.alphaTexture, tilledTexCoords).r;
}