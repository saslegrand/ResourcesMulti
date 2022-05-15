#version 450 core

in vec4 FragPos;

uniform float farPlane;
uniform vec3 lightPos;

void main()
{
	float lightToFragLength = length(FragPos.xyz - lightPos);

	// map to [0,1] by dividing by far plane
	lightToFragLength /= farPlane;

	// Write the depth value
	gl_FragDepth = lightToFragLength;
}