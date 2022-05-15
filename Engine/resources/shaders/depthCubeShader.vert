#version 450 core
layout (location = 0) in vec3 VertPos;

uniform mat4 model;

void main()
{
	// Transform vertice to world-space
	gl_Position = model * vec4(VertPos, 1.0);
}