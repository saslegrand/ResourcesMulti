#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormals;

uniform mat4 viewProj;
uniform mat4 model;

out vec2 TexCoords;
out vec3 normals;
out vec3 FragPos;

void main()
{
   gl_Position = (viewProj * model) * vec4(aPos, 1.0);
   TexCoords = aTexCoords;
   normals = (model * vec4(aNormals, 0.0)).xyz;
   FragPos = vec3(model * vec4(aPos, 1.0));
}