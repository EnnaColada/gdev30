#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;

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

    shaderTexCoord = vec2 ((vertexPosition.xy+1)/2);

    shaderPiece = 0.0f;
    if (vertexColor == vec3(0.702f, 0.129f, 0.075f) ||  //red
        vertexColor == vec3(0.561f, 0.086f, 0.000f) ||  //red shadow
        vertexColor == vec3(0.961f, 0.961f, 0.341f) ||  //yellow
        vertexColor == vec3(0.801f, 0.612f, 0.133f))    //yellow shadow
        shaderPiece = 1.0f; //fabric texture
    else if (vertexColor == vec3(0.000f,  1.000f,  1.000f)|| vertexColor == vec3 (0.000f,  0.500f,  0.500f))    //glass
        shaderPiece = 2.0f;
    else if (vertexColor == vec3(0.000f,  0.000f,  0.000f)){   //stars
        shaderPiece = 3.0f;
        shaderTexCoord = vec2 ((vertexPosition.xy+1)/25);

    }
        
    
}