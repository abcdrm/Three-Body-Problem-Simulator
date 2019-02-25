#version 330 core

//Default Variables
uniform sampler2D texture_diffuse1;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
//----------------

//From vertex shader
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

//To framebuffer
layout (location = 0) out vec3 color;

//light and view properties for shading
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 ViewPos;


void main()
{    
    vec4 ObjectColor = texture(texture_diffuse1, TexCoords);

    //ambient
    float ambientStrength = 1;
    vec3 ambient = Ka * LightColor * ambientStrength;  

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * Kd * LightColor;

    // Specular
    float specularStrength = 1;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = Ks * spec * LightColor * specularStrength;  

    vec3 result = (ambient + diffuse + specular) * vec3(ObjectColor);
    color = result;
}