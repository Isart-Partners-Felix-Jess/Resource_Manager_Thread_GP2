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
uniform int POINT_LIGHT_NBR;
#define MAX_POINT_LIGHTS 8
uniform PointLight pointLight[MAX_POINT_LIGHTS];
struct DirectionalLight
{
    vec3 direction;
    Light light;
};
uniform int DIRECTIONAL_LIGHT_NBR;
#define MAX_DIRECTIONAL_LIGHTS 8
uniform DirectionalLight directionalLight[MAX_DIRECTIONAL_LIGHTS];
//camera ref needed
uniform vec3 viewPos;

struct SpotLight
{
    vec3 direction;
    float cutoff;
    float outerCutoff;
    PointLight pointLight;
};
uniform int SPOT_LIGHT_NBR;
#define MAX_SPOT_LIGHTS 8
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];

//material members
struct Material 
{
    sampler2D diffuse2D;
    sampler2D specular2D;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;

//Process material
struct Sampled
{
vec3 Tex;
vec3 AmbTex;
vec3 DiffTex;
vec3 SpecTex;
};
Sampled sampled;
vec3 processLight(Light _light, vec3 _dir)
{
    //Ambient part
    vec3 ambient = _light.ambientStrength * _light.ambientColor * sampled.AmbTex;
    vec3 norm = normalize(Normal);

    //Diffuse part
    float diff = max(dot(norm, _dir), 0.0); //clamping the negative values
    vec3 diffuse = diff * _light.diffuseColor * sampled.DiffTex;

    //Specular part
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-_dir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = _light.specularStrength * spec * _light.specularColor * sampled.SpecTex;  
    
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
    float epsilon   = _spotLight.cutoff  - _spotLight.outerCutoff ;
    float intensity = clamp((theta - _spotLight.outerCutoff) / epsilon, 0.0, 1.0);    
      
    return processPointLight(_spotLight.pointLight) * intensity ;
}
void main()
{
   sampled.Tex = vec3(texture(material.diffuse2D, TexCoord));
   sampled.AmbTex = sampled.Tex * material.ambient;
   sampled.DiffTex = sampled.Tex * material.diffuse;
   sampled.SpecTex = vec3(texture(material.specular2D, TexCoord)) * material.specular;

    vec3 result = vec3(0.f,0.f,0.f);
    for(int i = 0; i < SPOT_LIGHT_NBR;i++)
       result += processSpotLight(spotLight[i]);
    for(int i = 0; i < DIRECTIONAL_LIGHT_NBR;i++)
        result += processDirectionalLight(directionalLight[i]);
    for(int i = 0; i < POINT_LIGHT_NBR;i++)
        result += processPointLight(pointLight[i]);
    FragColor = vec4(result * objectColor, 1.0);
}