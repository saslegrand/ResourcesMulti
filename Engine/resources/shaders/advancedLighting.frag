#version 450 core

#define NB_LIGHTS 8

struct Light
{
	vec4 position;
	vec4 ambientColor;
	vec4 diffuseColor;
	vec4 specularColor;

	vec3  direction;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outterCutOff;

	bool isSpot;
	bool isEnable;
};

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 emissiveColor;

	float opticalDensity;
	float shininess;
	float transparency;
	float illumination;
};

vec3 getShaderLight(Light lights[NB_LIGHTS], in Material mat, vec3 normals, vec3 FragPos, vec3 viewDir)
{
	vec3 ambient, diffuse, specular;
	vec3 lightDir, norm, reflectDir;
	float diff = 0.0;
	float spec = 0.0;
	float dist = 0.0;
	float attenuation = 0.0;

	ambient = vec3(0.0);
	diffuse = vec3(0.0);
	specular = vec3(0.0);

	for (int i = 0; i < NB_LIGHTS; i++)
	{
		Light light = lights[i];

		if (!light.isEnable)
			continue;

		float intensity = 1.0;

		if (light.position.w == 0.0)
			lightDir = normalize(light.direction);
		else
			lightDir = normalize(vec3(light.position) - FragPos);

		if (light.isSpot)
		{
			float theta = dot(lightDir, normalize(-light.direction));
			float epsilon = light.cutOff - light.outterCutOff;
			intensity = clamp((theta - light.outterCutOff) / epsilon, 0.0, 1.0);
		}

		// Calculate ambient light
		ambient += vec3(light.ambientColor) * mat.ambientColor;

		// Calculate diffuse light
		norm = normalize(normals);
		diff = max(dot(norm, lightDir), 0.0);
		diffuse += vec3(light.diffuseColor) * (diff * mat.diffuseColor);

		// Calculate specular light
		reflectDir = reflect(-lightDir, norm);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
		specular += vec3(light.specularColor) * (spec * mat.specularColor);

		// Attenuation (point and spot)
		dist = length(vec3(light.position) - FragPos);
		attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
		diffuse  *= attenuation;
		specular *= attenuation;

		// Intensity of spot light
		diffuse *= intensity;
		specular *= intensity;
		
		
	    /*if(blinn)
		{
		    vec3 halfwayDir = normalize(lightDir + viewDir);  
		    spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
		}
		else
		{
		    vec3 reflectDir = reflect(-lightDir, normal);
		    spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
		}*/
	}

	return (ambient + diffuse + specular);
}

void mat4ToLight(in mat4 lights[NB_LIGHTS][1], in mat4 attribsLights[NB_LIGHTS][1], out Light _lights[8])
{
	for (int i = 0; i < NB_LIGHTS; i++)
	{
		_lights[i].position	     = lights[i][0][0]; 
		_lights[i].ambientColor  = lights[i][0][1]; 
		_lights[i].diffuseColor  = lights[i][0][2]; 
		_lights[i].specularColor = lights[i][0][3];

		_lights[i].direction     = vec3(attribsLights[i][0][0]);
		_lights[i].constant		 = attribsLights[i][0][0][3];
		_lights[i].linear		 = attribsLights[i][0][1][0];
		_lights[i].quadratic     = attribsLights[i][0][1][1];
		_lights[i].cutOff		 = attribsLights[i][0][1][2];
		_lights[i].outterCutOff	 = attribsLights[i][0][1][3];
		_lights[i].isSpot	     = bool(attribsLights[i][0][2][0]);
		_lights[i].isEnable	     = bool(attribsLights[i][0][2][1]);
	}
}

void uniformToMaterial(in mat4 material, in vec4 materialAttribs, out Material _material)
{
	_material.ambientColor   = vec3(material[0]);
	_material.diffuseColor   = vec3(material[1]);
	_material.specularColor  = vec3(material[2]);
	_material.emissiveColor  = vec3(material[3]);

	_material.shininess = materialAttribs.x;
	_material.opticalDensity = materialAttribs.y;
	_material.transparency = materialAttribs.z;
	_material.illumination = materialAttribs.w;
}

uniform mat4 lights[NB_LIGHTS][1];
uniform mat4 attribsLights[NB_LIGHTS][1];
uniform mat4 material;
uniform vec4 materialAttribs;
uniform sampler2D textures[5][1];
uniform bool istextures[5][1];
uniform sampler2D texDif;
uniform vec3 viewPos;
out vec4 FragColor;
in vec2 TexCoords;
in vec3 normals;
in vec3 FragPos;

void main()
{
	Light _lights[NB_LIGHTS];
	Material mat;
	uniformToMaterial(material, materialAttribs, mat);
	mat4ToLight(lights, attribsLights, _lights);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 lightShader = getShaderLight(_lights, mat, normals, FragPos, viewDir);

	vec4 emissive = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 texValue = vec4(1.0, 1.0, 1.0, 1.0);

	if (istextures[0][0])
		texValue *= texture(textures[0][0], TexCoords);
	if (istextures[1][0])
		texValue *= texture(textures[1][0], TexCoords);
	if (istextures[2][0])
		texValue *= texture(textures[2][0], TexCoords);
	if (istextures[3][0])
		emissive  = texture(textures[3][0], TexCoords); //+ vec4(mat.emissiveColor, 1.0);

	FragColor = texValue * emissive; //+ vec4(lightShader, 1.0));

	if (istextures[4][0])
		FragColor.a = texture(textures[4][0], TexCoords).r;
}