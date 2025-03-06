/******************************************************************************
 * This is a really simple fragment shader that simply sets the output fragment
 * color to yellow.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;
out vec4 fragmentColor;

void main()
{
    //Exercise 1; Flip Horizontally
    vec2 shaderTexCoord = vec2(-shaderTexCoord.s, shaderTexCoord.t);

    //Exercise 3
    // fragmentColor = vec4(shaderColor, 1.0f) * texture(texture1, shaderTexCoord);

    //mixer
    vec4 colorA = texture(texture1, shaderTexCoord);
    vec4 colorB = texture(texture2, shaderTexCoord) * vec4(shaderColor, 1.0f);
    fragmentColor = mix(colorA, colorB, 0.5f * sin(time));

}

