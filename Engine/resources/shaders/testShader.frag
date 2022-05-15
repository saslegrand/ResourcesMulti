#version 450 core

out vec4 FragColor;
in float depth;

void main()
{
	FragColor = vec4(depth, depth, depth, 1.0);
}