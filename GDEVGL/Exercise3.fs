#version 330 core

in vec3 shaderColor;
out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(shaderColor, 1.0f);
}