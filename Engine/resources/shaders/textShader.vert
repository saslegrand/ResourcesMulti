#version 330 core
layout (location = 0) in vec4 vertPos;

out vec2 TexCoords;

uniform mat4 proj;

void main()
{
    gl_Position = proj * vec4(vertPos.xy, 0.0, 1.0);
    TexCoords = vertPos.zw;
}  