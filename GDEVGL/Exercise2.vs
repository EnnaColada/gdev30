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
uniform float time;
layout (location = 3) in float piece;
out vec3 shaderColor;
out vec2 shaderTexCoord;


void main()
{
    float t = clamp(time, 0, 1);

    if (piece == 0.0f) {
        gl_Position = vec4(vertexPosition.x-0.5f* sin(t), vertexPosition.y , vertexPosition.z, 1.0f);
        shaderColor = vertexColor;
    }
    else if (piece == 1.0f) {
        gl_Position = vec4(vertexPosition.x+0.5f* sin(t), vertexPosition.y, vertexPosition.z, 1.0f);
        shaderColor = vertexColor;
    }
    else {
        gl_Position = vec4(vertexPosition, 1.0f);
        shaderColor = vec3(vertexColor.r, vertexColor.g, vertexColor.b-1.0f);
    }
    
    
    shaderTexCoord = vertexTexCoord;
}
