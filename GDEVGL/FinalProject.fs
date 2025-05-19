#version 330 core

in vec3 worldSpacePos;
in vec3 worldSpaceNorm;
in vec3 objColor;
in vec2 shaderTexCoord;
flat in int shaderPiece;
out vec4 fragmentColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform float time;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform float specColor;

float ambColor = 0.3;
float constant = 1.0f;
float linear = 0.22f;
float quadratic = 0.2f;

void main()
{
    float lightWorldDistance = length(lightPosition - worldSpacePos);
    float attenuation = 1.0 / (constant + linear * lightWorldDistance + quadratic * (lightWorldDistance * lightWorldDistance));
    
    vec3 lightVec = normalize(lightPosition - worldSpacePos);
    vec3 norm = normalize(worldSpaceNorm);

    vec3 viewDir = normalize(lightPosition - worldSpacePos);
    vec3 reflectVec = reflect(-lightVec, norm);
    float spec = pow(max(dot(viewDir, reflectVec), 0.0), 32);
    vec3 specular = specColor * spec * lightColor; 

    float diffColor = max(dot(lightVec, norm), 0);

    vec3 finalColor = lightColor * (diffColor + ambColor + specular) * attenuation;

    if (shaderPiece == 1){
        fragmentColor = vec4(objColor, 1.0f)
                            * texture(texture1, shaderTexCoord)
                            * vec4(finalColor, 1.0f);
    }
    else if (shaderPiece == 2){
        fragmentColor = vec4(objColor * finalColor, 1.0f)
                            + texture(texture2, 
                            shaderTexCoord + vec2(0.2f*time, 0.2f*time))
                            * vec4(finalColor, 1.0f);
    } 
    else if (shaderPiece == 3){
        fragmentColor = vec4(objColor, 1.0f)
                            + texture(texture3, 
                            shaderTexCoord + vec2(0.05f*time, 0.05f*time));
    } 
    else if (shaderPiece == 4){
        vec4 texColor = texture(texture4, shaderTexCoord);
        if (texColor.a < 0.1)
            discard;
        else
            texColor.a = 0.7;
        
        fragmentColor = texColor;
    } 
    else 
        fragmentColor = vec4(objColor, 1.0f) * vec4(finalColor, 1.0f);
}