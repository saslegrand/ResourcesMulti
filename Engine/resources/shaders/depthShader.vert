#version 450 core
layout (location = 0) in vec3 VertPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
	gl_Position = lightSpaceMatrix * model * vec4(VertPos, 1.0);
}