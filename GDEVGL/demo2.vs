/******************************************************************************
 * This is a vertex shader that assumes that each vertex has a position and
 * color. The color of the vertex is further manipulated via a uniform, and
 * this color is passed to the fragment shader as an output.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 rotOrigin;
layout (location = 3) in vec2 effect;
uniform float glow;
uniform float time;
out vec3 shaderColor;
vec2 newCoords = vec2(vertexPosition.x - rotOrigin.x, vertexPosition.y - rotOrigin.y);

void main()
{
    //glow
    if (effect.x == 1.0f)
        shaderColor = vertexColor * glow;
    else
        shaderColor = vertexColor;
    
    //rotate counter-clockwise
    if (effect.y == 1.0f){
        vec2 rotatedCoords = vec2(cos(time) * newCoords.x - sin(time) * newCoords.y, sin(time) * newCoords.x + cos(time) * newCoords.y);
        gl_Position = vec4(rotatedCoords.x + rotOrigin.x, rotatedCoords.y + rotOrigin.y, vertexPosition.z, 1.0f);
    }
    //rotate clockwise
    else if(effect.y == 2.0f){
        vec2 rotatedCoords = vec2(cos(time) * newCoords.x + sin(time) * newCoords.y, -sin(time) * newCoords.x + cos(time) * newCoords.y);
        gl_Position = vec4(rotatedCoords.x + rotOrigin.x, rotatedCoords.y + rotOrigin.y, vertexPosition.z, 1.0f);
    }
    else
        gl_Position = vec4(vertexPosition.x,vertexPosition.y,vertexPosition.z, 1.0f);
}
