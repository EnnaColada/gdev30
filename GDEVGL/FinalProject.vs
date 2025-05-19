#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 projMatrix;
uniform mat4 modMatrix;
uniform mat4 norMatrix;
uniform int forceRed;

out vec3 shaderColor;
out vec2 shaderTexCoord;
flat out int shaderPiece;

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
    shaderPiece = 0;

    if (vertexColor == vec3(0.961f, 0.961f, 0.341f)) {                              //yellow
        shaderPiece = 1;
        objColor = (forceRed == 1)? vec3(0.702f, 0.129f, 0.075f) : vertexColor;     //red
    } else if (vertexColor == vec3(0.801f, 0.612f, 0.133f)) {                       //yellow shadow
        shaderPiece = 1;
        objColor = (forceRed == 1)? vec3(0.561f, 0.086f, 0.000f) : vertexColor;     //red shadow
    } 
    else if (vertexColor == vec3(0.0f,  1.0f,  1.0f) || vertexColor == vec3 (0.0f,  0.5f,  0.5f))    //glass
        shaderPiece = 2;
    else if (vertexColor == vec3(0.0f,  0.0f,  0.0f)){   //stars
        shaderPiece = 3;
        shaderTexCoord = vec2 ((vertexPosition.xy+1)/25);
    }
    else if (vertexColor == vec3(1.0f,  1.0f,  1.0f)){    //blood
        shaderPiece = 4;
        shaderTexCoord = vec2 ((vertexPosition.x-0.450f)/0.4, (vertexPosition.y+0.300f)/0.4);
    }
}