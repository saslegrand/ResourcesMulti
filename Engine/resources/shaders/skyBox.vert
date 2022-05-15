#version 330 core
layout (location = 0) in vec3 vertPos;

out vec3 TexCoords;

uniform mat4 viewProj;

void main()
{
    TexCoords = vertPos;
    gl_Position = viewProj * vec4(vertPos, 1.0);
}  