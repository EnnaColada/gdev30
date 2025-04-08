#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in float vertexPiece;

uniform mat4 matrix;
out vec3 shaderColor;
out vec2 shaderTexCoord;
out float shaderPiece;


void main()
{
    gl_Position = matrix * vec4(vertexPosition, 1.0f);
    shaderColor = vertexColor;

    shaderTexCoord = vec2 ((vertexPosition.xy+1)/2);
    shaderPiece = vertexPiece;
}