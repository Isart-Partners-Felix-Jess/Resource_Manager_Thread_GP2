#include <Light.hpp>

#include <Shader.hpp>
void Light::InitShader(std::string _lightType, Shader& _lightShader)
{
	_lightShader.SetVec3(_lightType+"light.ambientColor", ambient);					
	_lightShader.SetVec3(_lightType+"light.diffuseColor", diffuse);				 
	_lightShader.SetVec3(_lightType+"light.specularColor", specular);
	_lightShader.SetFloat(_lightType+"light.ambientStrength", ambientStrength);
	_lightShader.SetFloat(_lightType+"light.specularStrength", specularStrength);
}

void PointLight::InitShader(Shader& _lightShader)
{
	light.InitShader("pointLight.", _lightShader);
	_lightShader.SetVec3("pointLight.position", position);
	_lightShader.SetFloat("pointLight.constant", constant);
	_lightShader.SetFloat("pointLight.linear", linear);
	_lightShader.SetFloat("pointLight.quadratic", quadratic);
}

void DirectionalLight::InitShader(Shader& _lightShader)
{
	light.InitShader("directionalLight.", _lightShader);
	_lightShader.SetVec3("directionalLight.light.position", direction);
}
