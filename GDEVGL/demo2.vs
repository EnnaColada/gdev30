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
layout (location = 2) in vec2 vertexAnim;
uniform float glow;
out vec3 shaderColor;

void main()
{
    gl_Position = vec4(vertexPosition.x,vertexPosition.y,vertexPosition.z, 1.0f);

    if (vertexAnim.x == 1) {
        shaderColor = vertexColor * glow;
    }
    else {
        shaderColor = vertexColor;
    }
}
