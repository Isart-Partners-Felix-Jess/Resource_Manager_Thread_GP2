#include <Light.hpp>

#include <Shader.hpp>
void Light::InitShader(Shader& _lightShader)
{
	_lightShader.SetVec3("light.position", position);
	_lightShader.SetVec3("light.ambientColor", ambient);
	_lightShader.SetVec3("light.diffuseColor", diffuse);
	_lightShader.SetVec3("light.specularColor", specular);
	_lightShader.SetFloat("light.ambientStrength", ambientStrength);
	_lightShader.SetFloat("light.specularStrength", specularStrength);
}