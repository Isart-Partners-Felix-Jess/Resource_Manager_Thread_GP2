#version 330 core
in vec3 Normal;
in vec3 FragPos;  
out vec4 FragColor;
in vec2 TexCoords;

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

struct SpotLight
{
    vec3 direction;
    float cutoff;
    PointLight point;
};
uniform SpotLight spotLight;


//material members
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;
vec3 processLight(Light _light, vec3 _dir)
{
    //Ambient part
    vec3 ambient = _light.ambientStrength * _light.ambientColor * material.ambient;
    vec3 norm = normalize(Normal);

    //Diffuse part
    float diff = max(dot(norm, _dir), 0.0); //clamping the negative values
    vec3 diffuse = diff * _light.diffuseColor * material.diffuse;

    //Specular part
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-_dir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = _light.specularStrength * spec * _light.specularColor * material.specular;  
    
    //Combine
    vec3 result = (ambient + diffuse + specular) * objectColor;
    return result;
}

void processPointLight(PointLight _pointLight)
{
    float distance    = length(_pointLight.position - FragPos);
    float attenuation = 1.0 / (_pointLight.constant + _pointLight.linear * distance + 
    		    _pointLight.quadratic * (distance * distance));   

    //Compute directionDiffuse part
    vec3 lightDir = normalize(_pointLight.position - FragPos);  

    //Combine
    vec3 result = processLight(_pointLight.light, lightDir) * attenuation;
    FragColor = vec4(result, 1.0);
}


void processDirectionalLight(DirectionalLight _directionalLight)
{
    //Combine
    vec3 result = processLight(_directionalLight.light, _directionalLight.direction);
    FragColor = vec4(result, 1.0);
}
void processSpotLight(SpotLight _spotLight)
{
    vec3 lightDir = normalize(_spotLight.point.position - FragPos);
    float theta = dot(lightDir,_spotLight.direction);
    
    if(theta > _spotLight.cutoff) 
    {       
      processPointLight(_spotLight.point);
    }
    else  // else, use ambient light so scene isn't completely dark outside the spotlight.
      FragColor = vec4(_spotLight.point.light.ambientColor,1.0 );//* vec3(texture(material.diffuse, TexCoords)), 1.0);
}
void main()
{
    processDirectionalLight(directionalLight);
    //processPointLight(pointLight);
}