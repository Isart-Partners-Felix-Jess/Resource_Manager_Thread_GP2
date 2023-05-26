#version 330 core
in vec3 Normal;
in vec3 FragPos;  
out vec4 FragColor;
  
uniform vec3 objectColor;
//ref to light(s)
//light members create struct light here
struct Light
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float ambientStrength;
    float specularStrength;
};
struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    Light light;
};
uniform PointLight pointLight;
struct DirectionalLight
{
    vec3 direction;
    Light light;
};
uniform DirectionalLight directionalLight;
//camera ref needed
uniform vec3 viewPos;

//material members
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

void processPointLight()
{
    float distance    = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + 
    		    pointLight.quadratic * (distance * distance));   
    //Ambient part
    vec3 ambient = pointLight.light.ambientStrength * pointLight.light.ambientColor * material.ambient;
    vec3 norm = normalize(Normal);

    //Diffuse part
    vec3 lightDir = normalize(pointLight.position - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0); //clamping the negative values
    vec3 diffuse = diff * pointLight.light.diffuseColor * material.diffuse;

    //Specular part
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pointLight.light.specularStrength * spec * pointLight.light.specularColor * material.specular;  
    
    //Combine
    vec3 result = (ambient + diffuse + specular) * objectColor * attenuation;
    FragColor = vec4(result, 1.0);
}
void processDirectionalLight()
{
    //Ambient part
    vec3 ambient = directionalLight.light.ambientStrength * directionalLight.light.ambientColor * material.ambient;
    vec3 norm = normalize(Normal);

    //Diffuse part
    float diff = max(dot(norm, directionalLight.direction), 0.0); //clamping the negative values
    vec3 diffuse = diff * directionalLight.light.diffuseColor * material.diffuse;

    //Specular part
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-directionalLight.direction, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionalLight.light.specularStrength * spec * directionalLight.light.specularColor * material.specular;  
    
    //Combine
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}

void main()
{
    processDirectionalLight();
    //processPointLight();
}