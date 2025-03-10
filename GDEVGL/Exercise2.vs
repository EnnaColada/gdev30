/******************************************************************************
 * This is a really simple vertex shader that simply sets the output vertex's
 * position to be the same as the input.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in float piece;
uniform float time;
out vec3 shaderColor;
out vec2 shaderTexCoord;


void main()
{
    float t = clamp(sin(time)+0.5, 0, 1);
    vec3 zoomedPosition = vec3(vertexPosition*3);
    vec3 scaledVertexPosition = vertexPosition*0.5;

    if (sin(time+0.52)+0.5 > 0){
        
        gl_Position = vec4(zoomedPosition*(sin(time)+0.5), 1.0f);
    }
    else{
        
        if (piece == 0.0f) {
            gl_Position = vec4(scaledVertexPosition, 1.0f);
            shaderColor = vertexColor;
        }
        else if (piece == 1.0f){
            gl_Position = vec4(scaledVertexPosition.x-0.5f* sin(t), scaledVertexPosition.y , scaledVertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        else if (piece == 2.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.5f* sin(t), scaledVertexPosition.y, vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        else {
            gl_Position = vec4(vertexPosition * 0.5, 1.0f);
            shaderColor = vec3(vertexColor.r, vertexColor.g, vertexColor.b-1.0f);

        }
    }
    shaderTexCoord = vertexTexCoord;
    
}
