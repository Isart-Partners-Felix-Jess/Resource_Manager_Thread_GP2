#version 330 core

in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;  
in vec2 TexCoord;

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

struct SpotLight
{
    vec3 direction;
    float cutoff;
    float outerCutoff;
    PointLight pointLight;
};
uniform SpotLight spotLight;


//material members
struct Material {
    sampler2D diffuse2D;
    sampler2D specular2D;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;
//Process material
vec3 sampledTex = vec3(texture(material.diffuse2D, TexCoord));
vec3 sampledAmbTex = sampledTex * material.ambient;
vec3 sampledDiffTex = sampledTex * material.diffuse;
vec3 sampledSpecTex = vec3(texture(material.specular2D, TexCoord)) * material.specular;

vec3 processLight(Light _light, vec3 _dir)
{
    //Ambient part
    vec3 ambient = _light.ambientStrength * _light.ambientColor * sampledAmbTex;
    vec3 norm = normalize(Normal);

    //Diffuse part
    float diff = max(dot(norm, _dir), 0.0); //clamping the negative values
    vec3 diffuse = diff * _light.diffuseColor * sampledDiffTex;

    //Specular part
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-_dir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = _light.specularStrength * spec * _light.specularColor * sampledSpecTex;  
    
    //Combine
    vec3 result = (ambient + diffuse + specular) * objectColor;
    return result;
}

vec3 processPointLight(PointLight _pointLight)
{
    float distance    = length(_pointLight.position - FragPos);
    float attenuation = 1.0 / (_pointLight.constant + _pointLight.linear * distance + 
    		    _pointLight.quadratic * (distance * distance));   

    //Compute directionDiffuse part
    vec3 lightDir = normalize(_pointLight.position - FragPos);  

    //Combine
    vec3 result = processLight(_pointLight.light, lightDir) * attenuation;
    return result;
}


vec3 processDirectionalLight(DirectionalLight _directionalLight)
{
    //Combine
    vec3 result = processLight(_directionalLight.light, _directionalLight.direction);
    return result;
}
vec3 processSpotLight(SpotLight _spotLight)
{
    vec3 lightDir = normalize(_spotLight.pointLight.position - FragPos);
    float theta = dot(lightDir,_spotLight.direction);
    float epsilon   = _spotLight.outerCutoff - _spotLight.cutoff ;
    //float epsilon   = innerCutoff - _spotLight.outerCutoff;
    float intensity = clamp((theta - _spotLight.outerCutoff) / epsilon, _spotLight.pointLight.light.ambientStrength, 1.0);    
    
    //if(theta > _spotLight.cutoff) 
    {    //   _spotLight.pointLight.light.ambientStrength/=intensity;
        return processPointLight(_spotLight.pointLight) * intensity;
        //_spotLight.pointLight.light.ambientStrength *=intensity;
    }
   // else  // else, use ambient light so scene isn't completely dark outside the spotlight.
    //  return _spotLight.pointLight.light.ambientColor * _spotLight.pointLight.light.ambientStrength * sampledAmbTex;
}
void main()
{
    vec3 result;
   // result += processDirectionalLight(directionalLight);
    //result += processPointLight(pointLight);
    result += processSpotLight(spotLight);

    FragColor = vec4(result * objectColor, 1.0);
}