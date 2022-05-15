#version 450 core
layout (location = 0) in vec3 VertPos;

uniform mat4 viewProj;
uniform mat4 model;

void main()
{
	gl_Position = viewProj * model * vec4(VertPos, 1.f);
}