#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in float vertexPiece;
layout (location = 3) in vec3 vertexNormal;

uniform mat4 projMatrix;
uniform mat4 modMatrix;
uniform mat4 norMatrix;

out vec3 shaderColor;
out vec2 shaderTexCoord;
out float shaderPiece;

out vec3 worldSpacePos;
out vec3 worldSpaceNorm;
out vec3 objColor;


void main()
{
    worldSpacePos = (modMatrix * vec4(vertexPosition, 1.0f)).xyz;
    worldSpaceNorm = (norMatrix * vec4(vertexNormal, 1.0f)).xyz;
    objColor = vertexColor;
    
    gl_Position = projMatrix * vec4(worldSpacePos, 1.0f);
    shaderColor = vertexColor;

    shaderTexCoord = vec2 ((vertexPosition.xy+1)/2);
    shaderPiece = vertexPiece;

    
}