#version 450 core
layout (location = 0) in vec3 VertPos;
layout (location = 1) in vec3 VertTexCoords;
layout (location = 2) in vec3 VertNormal;

out vec3 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 viewProj;
uniform mat4 model;

void main()
{
	gl_Position = viewProj * model * vec4(VertPos, 1.0);
	FragPos = vec3(model * vec4(VertPos, 1.0));
	
	TexCoord = VertTexCoords;

	Normal = transpose(inverse(mat3(model))) * VertNormal;
}