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
    // float cosAbs = abs(cos(time));
    // float sinAbs = abs(sin(time));
    // float PI = 3.14159265358;
    float g = sin(time) * 10/360 + 10/360;
    float cosAbs = cos(g);
    float sinAbs = sin(g);

    float flapSpeed = 3.0;  // Controls how fast the butterfly flaps
    float flapAngle = sin(time * flapSpeed) * 0.3; // Oscillating angle for wing movement
    float scaleFactor = sin(time * flapSpeed);
    float cosFlap = cos(flapAngle);
    float sinFlap = sin(flapAngle);


    //glow
    if (effect.x == 1.0f)
        shaderColor = vertexColor * glow;
    else
        shaderColor = vertexColor;
    
    //rotate counter-clockwise
    if (effect.y == 1.0f){
        // vec2 rotatedCoords = vec2(clamp((cosAbs * newCoords.x - sinAbs * newCoords.y),newCoords.x*0.9,newCoords.x), clamp((sinAbs * newCoords.x + cosAbs * newCoords.y),newCoords.y*0.9,newCoords.y));
        vec2 rotatedCoords = vec2(cosAbs * newCoords.x - sinAbs * newCoords.y, sinAbs * newCoords.x + cosAbs * newCoords.y);
        gl_Position = vec4(rotatedCoords.x + rotOrigin.x, rotatedCoords.y + rotOrigin.y, vertexPosition.z, 1.0f);
    }
    //rotate clockwise
    else if(effect.y == 2.0f){
        vec2 rotatedCoords = vec2(cosAbs * newCoords.x + sinAbs * newCoords.y, -sinAbs * newCoords.x + cosAbs * newCoords.y);
        gl_Position = vec4(rotatedCoords.x + rotOrigin.x, rotatedCoords.y + rotOrigin.y, vertexPosition.z, 1.0f);
    }
    else if(effect.y == 3.0f){
        //if it doesn't work out ⬇️
        //gl_Position = vec4(rotOrigin.x * cos(time) + rotOrigin.y * sin(time) + vertexPosition.x,
        //                  -rotOrigin.x * cos(time) + rotOrigin.y * sin(time) + vertexPosition.y, vertexPosition.z, 1.0f);

        // vec2 rotatedCoords = vec2(rotOrigin.x * cos(time) + rotOrigin.y * sin(time) + vertexPosition.x,
        //                   -rotOrigin.x * cos(time) + rotOrigin.y * sin(time) + vertexPosition.y);
        // vec2 flapCoords = vec2(cosFlap * newCoords.x + sinFlap * newCoords.y, 
        //                          -sinFlap * newCoords.x + cosFlap * newCoords.y);
      
        // //vec2 finalCoords = vec2((rotatedCoords + flapCoords)/2);
        // float verticalMotion = sin(time * flapSpeed) * 0.1;

        // gl_Position = vec4(finalCoords.x + rotOrigin.x, 
        //                    finalCoords.y + rotOrigin.y + verticalMotion, 
        //                    vertexPosition.z, 1.0f);
        
        vec2 flapCoords = vec2(cosFlap * newCoords.x - sinFlap * newCoords.y * scaleFactor, 
                               sinFlap * newCoords.x + cosFlap * newCoords.y * scaleFactor);
        // vec2 rotatedCoords = vec2(rotOrigin.x * cos(time) + rotOrigin.y * sin(time) + vertexPosition.x,
        //                          -rotOrigin.x * cos(time) + rotOrigin.y * sin(time) + vertexPosition.y);   
        // vec2 final = (flapCoords + rotatedCoords)/2;                    
        float verticalMotion = sin(time * flapSpeed) * 0.1; 

        gl_Position = vec4(flapCoords.x + rotOrigin.x, flapCoords.y + rotOrigin.y + verticalMotion, 
                           vertexPosition.z, 1.0f);
    }
    else
        gl_Position = vec4(vertexPosition.x,vertexPosition.y,vertexPosition.z, 1.0f);

}
