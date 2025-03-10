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
uniform float glow;
out vec3 shaderColor;
out vec2 shaderTexCoord;


void main()
{
    float testTime = sin((time+2.5)/2)*2;
    float t = clamp(testTime, -2, 0);
    vec3 scaledVertexPosition = vertexPosition*0.5;

    if (testTime > 0.565){
        gl_Position = vec4((vertexPosition*.95)*clamp(abs(testTime), 0, 1), 1.0f);
    }
    // float testTime = sin(time+1)*2;
    // float t = clamp(testTime, -2, 0);
    // vec3 scaledVertexPosition = vertexPosition*0.5;

    // if (testTime > 0.2){
    //     gl_Position = vec4((vertexPosition*.95)*abs(testTime), 1.0f);
    //     shaderColor = vertexColor*glow;
    // }
    else{
        if (piece == 0.0f) {
            gl_Position = vec4(scaledVertexPosition, 1.0f);
            shaderColor = vertexColor;
        }
         //top right piece (poly3)
        else if (piece == 1.0f) {
            gl_Position = vec4(scaledVertexPosition.x-0.3f* sin(t), scaledVertexPosition.y-0.3f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //middle top right piece (poly4)
        else if (piece == 2.0f) {
            gl_Position = vec4(scaledVertexPosition.x-0.6f* sin(t), scaledVertexPosition.y-0.2f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //middle bottom right piece (poly5)
        else if (piece == 3.0f) {
            gl_Position = vec4(scaledVertexPosition.x-0.65f* sin(t), scaledVertexPosition.y-0.1f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //middle piece with diamond (poly6)
        else if (piece == 4.0f) {
            gl_Position = vec4(scaledVertexPosition.x-0.4f* sin(t), scaledVertexPosition.y+0.05f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //bottom middle piece (poly7)
        else if (piece == 5.0f) {
            gl_Position = vec4(scaledVertexPosition.x-0.2f*sin(t), scaledVertexPosition.y+0.5f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //bottom right piece (poly8)
        else if (piece == 6.0f) {
            gl_Position = vec4(scaledVertexPosition.x-0.45f* sin(t), scaledVertexPosition.y+0.2f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //bottom left piece (poly9)
        else if (piece == 7.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.2f*sin(t), scaledVertexPosition.y+0.25f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //middle bottom left inner piece (poly10)
        else if (piece == 8.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.25f*sin(t), scaledVertexPosition.y+0.1f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //middle piece without diamond (poly11)
        else if (piece == 9.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.35f*sin(t), scaledVertexPosition.y+0.05f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
         //left middle bottom piece (poly12)
        else if (piece == 10.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.5f*sin(t), scaledVertexPosition.y+0.05f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //left middle top piece (poly13)
        else if (piece == 11.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.5f*sin(t), scaledVertexPosition.y-0.15f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //top left piece (poly14)
        else if (piece == 12.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.4f*sin(t), scaledVertexPosition.y-0.3f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-1.0f, vertexColor.g, vertexColor.b);
        }
        //topmost left piece (poly15)
        else if (piece == 13.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.2f*sin(t), scaledVertexPosition.y-0.4f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor;
        }
        //sun
        else if (piece == 14.0f) {
            gl_Position = vec4(scaledVertexPosition.x, scaledVertexPosition.y-0.1f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor*glow;
        }
        // else {
        //     gl_Position = vec4(vertexPosition * 0.5, 1.0f);
        //     shaderColor = vec3(vertexColor.r-1.0f, vertexColor.g, vertexColor.b);
        // }
    }
    shaderTexCoord = vertexTexCoord;
    
}
