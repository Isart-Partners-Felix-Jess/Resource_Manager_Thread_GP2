#version 330 core
in vec3 Normal;
in vec3 FragPos;  
out vec4 FragColor;
  
uniform vec3 objectColor;
//ref to light(s)
//light members create struct light here

uniform vec3 lightPos;  
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float ambientStrength;
uniform float specularStrength;

//camera ref needed
uniform vec3 viewPos;

void main()
{

    //material members
    int shininess = 32;

    //Ambient part
    vec3 ambient = ambientStrength * ambientColor;
    vec3 norm = normalize(Normal);

    //Diffuse part
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0); //clamping the negative values
    vec3 diffuse = diff * diffuseColor;

    //Specular part
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * specularColor;  
    
    //Combine
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}