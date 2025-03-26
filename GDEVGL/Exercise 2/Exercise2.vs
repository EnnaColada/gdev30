#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in float piece;
uniform float time;
uniform float glow;
out vec3 shaderColor;
out vec2 shaderTexCoord;

vec2 rotate(vec2 position, vec2 origin, float g){
    vec2 newCoords = position - origin;
    vec2 rotatedCoords = vec2(cos(g) * newCoords.x - sin(g) * newCoords.y,
                              sin(g) * newCoords.x + cos(g) * newCoords.y);
    return rotatedCoords + origin;
}

void main()
{
    // float newTime = sin(time+1)*2;
    float newTime = sin((time+2.5)/2)*2;
    float t = clamp(newTime, -2, 0);
    vec3 scaledVertexPosition = vertexPosition*0.5;
    vec2 rotOrigin; 
    float g = (sin(t) * 0.33);

    // zooming in and out
    if (newTime > 0.505){
        gl_Position = vec4((vertexPosition*0.95)*clamp(abs(newTime), 0, 1), 1.0f);
        shaderColor = vertexColor * (piece == 0.0f? 1:sin(newTime));               // Background pieces (piece == 0.0f) remain unchanged; 
    }                                                                              // other pieces darken the further they move

    // // not the one I submitted
    // if (newTime > 0.2){
    //     gl_Position = vec4((vertexPosition*0.95)*clamp(abs(newTime), 0, 2), 1.0f);
    //     shaderColor = vertexColor * (piece == 0.0f? 1:sin(newTime));            
    // }        

    // Shattering effect: pieces move, rotate, and change color before returning to their original position
    else{
        //top right piece (poly3)
        if (piece == 1.0f) {
            rotOrigin = vec2(0.228f, 0.764f);
            vec2 rotatedCoords = rotate(scaledVertexPosition.xy, rotOrigin, g);
            gl_Position = vec4(rotatedCoords.x-0.3f* sin(t), rotatedCoords.y-0.3f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-0.3f*sin(t), vertexColor.g+0.2f*sin(t), vertexColor.b-0.3f*sin(t));
        }
        //middle top right piece (poly4)
        else if (piece == 2.0f) {
            rotOrigin = vec2(0.381f, 0.357f);
            vec2 rotatedCoords = rotate(scaledVertexPosition.xy, rotOrigin, -g);
            gl_Position = vec4(rotatedCoords.x-0.45f* sin(t), rotatedCoords.y-0.3f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-0.3f*sin(t), vertexColor.g+0.2f*sin(t), vertexColor.b+0.6f*sin(t));
        }
        //middle bottom right piece (poly5)
        else if (piece == 3.0f) {
            gl_Position = vec4(scaledVertexPosition.x-0.65f* sin(t), scaledVertexPosition.y-0.1f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-0.3f*sin(t), vertexColor.g-0.2f*sin(t), vertexColor.b+0.6f*sin(t));
        }
        //middle piece with diamond (poly6)
        else if (piece == 4.0f) {
            rotOrigin = vec2(0.157f, -0.333f);
            vec2 rotatedCoords = rotate(scaledVertexPosition.xy, rotOrigin, -g);
            gl_Position = vec4(rotatedCoords.x-0.4f* sin(t), rotatedCoords.y+0.05f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-4.0f*sin(t), vertexColor.g, vertexColor.b-0.1*sin(t));
        }
        //bottom middle piece (poly7)
        else if (piece == 5.0f) {
            gl_Position = vec4(scaledVertexPosition.x-0.2f*sin(t), scaledVertexPosition.y+0.5f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r, vertexColor.g-0.7f*sin(t), vertexColor.b-0.3f*sin(t));
        }
        //bottom right piece (poly8)
        else if (piece == 6.0f) {
            rotOrigin = vec2(0.004f,-0.806f);
            vec2 rotatedCoords = rotate(scaledVertexPosition.xy, rotOrigin, g);
            gl_Position = vec4(rotatedCoords.x-0.45f* sin(t), rotatedCoords.y+0.2f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-1.0f*sin(t), vertexColor.g, vertexColor.b-0.4f*sin(t));
        }
        //bottom left piece (poly9)
        else if (piece == 7.0f) {
            rotOrigin = vec2(-0.247f, -0.913f);
            vec2 rotatedCoords = rotate(scaledVertexPosition.xy, rotOrigin, -g);
            gl_Position = vec4(rotatedCoords.x+0.25f*sin(t), rotatedCoords.y+0.3f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-0.3f*sin(t), vertexColor.g+0.2f*sin(t), vertexColor.b+0.6f*sin(t));
        }
        //middle bottom left inner piece (poly10)
        else if (piece == 8.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.25f*sin(t), scaledVertexPosition.y+0.1f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-0.4f*sin(t), vertexColor.g-0.5f*sin(t), vertexColor.b+0.6f*sin(t));
        }
        //middle piece without diamond (poly11)
        else if (piece == 9.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.35f*sin(t), scaledVertexPosition.y+0.05f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-0.36f*sin(t), vertexColor.g, vertexColor.b-0.45f*sin(t));
        }
         //left middle bottom piece (poly12)
        else if (piece == 10.0f) {
            rotOrigin = vec2(-0.4f,-0.4f);
            vec2 rotatedCoords = rotate(scaledVertexPosition.xy, rotOrigin, -g);
            gl_Position = vec4(rotatedCoords.x+0.5f*sin(t), rotatedCoords.y+0.05f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r+0.3f*sin(t), vertexColor.g-1.0f*sin(t), vertexColor.b+0.5f*sin(t));
        }
        //left middle top piece (poly13)
        else if (piece == 11.0f) {
            rotOrigin = vec2(-0.34f,0.196f);
            vec2 rotatedCoords = rotate(scaledVertexPosition.xy, rotOrigin, g);
            gl_Position = vec4(rotatedCoords.x+0.5f*sin(t), rotatedCoords.y-0.15f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-0.3f*sin(t), vertexColor.g-0.7f*sin(t), vertexColor.b);
        }
        //top left piece (poly14)
        else if (piece == 12.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.4f*sin(t), scaledVertexPosition.y-0.3f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r-1.0f*sin(t), vertexColor.g, vertexColor.b);
        }
        //topmost left piece (poly15)
        else if (piece == 13.0f) {
            gl_Position = vec4(scaledVertexPosition.x+0.2f*sin(t), scaledVertexPosition.y-0.4f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vec3(vertexColor.r, vertexColor.g-0.4f*sin(t), vertexColor.b-0.2f*sin(t));
        }
        //glowing sun 
        else if (piece == 14.0f) {
            gl_Position = vec4(scaledVertexPosition.x, scaledVertexPosition.y-0.2f* sin(t), vertexPosition.z, 1.0f);
            shaderColor = vertexColor*glow;
        }
    }
    shaderTexCoord = vec2 ((vertexPosition.xy+1)/2);
}
