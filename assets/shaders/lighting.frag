#version 330 core
in vec3 Normal;
in vec3 FragPos;  
out vec4 FragColor;
  
uniform vec3 objectColor;
//ref to light(s)
//light members create struct light here
struct Light
{
vec3 position;  
vec3 ambientColor;
vec3 diffuseColor;
vec3 specularColor;
float ambientStrength;
float specularStrength;
};
uniform Light light;
//camera ref needed
uniform vec3 viewPos;
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

void main()
{

    //material members

    //Ambient part
    vec3 ambient = light.ambientStrength * light.ambientColor * material.ambient;
    vec3 norm = normalize(Normal);

    //Diffuse part
    vec3 lightDir = normalize(light.position - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0); //clamping the negative values
    vec3 diffuse = diff * light.diffuseColor * material.diffuse;

    //Specular part
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specularStrength * spec * light.specularColor * material.specular;  
    
    //Combine
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}