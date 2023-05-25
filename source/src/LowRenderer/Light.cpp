#include <Light.hpp>

#include <Shader.hpp>
void Light::InitShader(Shader& _lightShader)
{
	_lightShader.SetVec3("lightPos", position);
	_lightShader.SetVec3("ambientColor", ambient);
	_lightShader.SetVec3("diffuseColor", diffuse);
	_lightShader.SetVec3("specularColor", specular);
	_lightShader.SetFloat("ambientStrength", ambientStrength);
	_lightShader.SetFloat("specularStrength", specularStrength);
}