#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
uniform sampler2D texture1; //main img
uniform sampler2D texture2; //cracked glass
uniform sampler2D texture3; //disp map
uniform float time;
out vec4 fragmentColor;

void main()
{
    float t = sin((time+2.5)/2)*2;

    vec4 dispMap = texture(texture3, shaderTexCoord + vec2(0.2f*t, 0.2f*t));
    vec2 disp = vec2(1, 1)*(dispMap.r*2 - 1) * 0.01f;
    
    vec4 colorA = texture(texture1, shaderTexCoord + disp);
    vec4 colorB = texture(texture2, shaderTexCoord + disp);
    // Blends the main texture with the white parts of the cracked glass texture
    vec4 finalColor = colorA + colorB.r;                
    
    // Applies the blending effect based on animation state
    fragmentColor = vec4(shaderColor, 1.0f) * (t > 0.505? colorA: finalColor);
}

