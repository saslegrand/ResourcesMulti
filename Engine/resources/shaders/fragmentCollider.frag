#version 450 core

uniform vec3 viewPos;
uniform vec3 color;

out vec4 FragColor;

void main()
{
	vec3 test = viewPos;
	FragColor = vec4(color, 1.0);//color;
}