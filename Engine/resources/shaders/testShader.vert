#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoords;
layout (location = 2) in vec3 aNormals;

uniform mat4 viewProj;
uniform mat4 model;

out float depth;

void main()
{
   gl_Position = viewProj * model * vec4(aPos, 1.0);
   depth = gl_Position.z / 20.0;
}