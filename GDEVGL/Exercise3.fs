#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
in float shaderPiece;
out vec4 fragmentColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;



void main()
{
    if (shaderPiece == 0.0f)
        fragmentColor = vec4(shaderColor, 1.0f)*texture(texture1, shaderTexCoord);
    else
        fragmentColor = vec4(shaderColor, 1.0f)+texture(texture2, shaderTexCoord+ vec2(0.2f*time, 0.2f*time));
}