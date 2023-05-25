#version 330 core
in vec3 Normal;
in vec3 FragPos;  
out vec4 FragColor;
  
uniform vec3 objectColor;
//ref to light(s)
uniform vec3 lightColor;
uniform vec3 lightPos;  
//camera ref needed
uniform vec3 viewPos;

void main()
{
    //light members
    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    //material members
    int shininess = 32;

    //Ambient part
    vec3 ambient = ambientStrength * lightColor;
    vec3 norm = normalize(Normal);

    //Diffuse part
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0); //clamping the negative values
    vec3 diffuse = diff * lightColor;

    //Specular part
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;  
    
    //Combine
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}