#version 450 core

in vec3 TexCoords;

uniform sampler2D diffuseTex;
uniform vec4 color;

uniform vec2 tilling;

out vec4 FragColor;

vec2 getTilledTexCoords()
{
	return TexCoords.st * tilling.x + tilling.y;
}

void main()
{
	FragColor = color * texture(diffuseTex, getTilledTexCoords());
}