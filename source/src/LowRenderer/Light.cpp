#include <Light.hpp>
#define _USE_MATH_DEFINES

#include <Shader.hpp>
void Light::InitShader(std::string _lightType, Shader& _lightShader)
{
	_lightShader.SetVec3(_lightType+"light.ambientColor", ambient);					
	_lightShader.SetVec3(_lightType+"light.diffuseColor", diffuse);				 
	_lightShader.SetVec3(_lightType+"light.specularColor", specular);
	_lightShader.SetFloat(_lightType+"light.ambientStrength", ambientStrength);
	_lightShader.SetFloat(_lightType+"light.specularStrength", specularStrength);
}

void PointLight::InitShader(Shader& _lightShader,std::string _structType )
{
	light.InitShader(_structType+"pointLight.", _lightShader);
	_lightShader.SetVec3(_structType+"pointLight.position", position);
	_lightShader.SetFloat(_structType+"pointLight.constant", constant);
	_lightShader.SetFloat(_structType+"pointLight.linear", linear);
	_lightShader.SetFloat(_structType+"pointLight.quadratic", quadratic);
}

void DirectionalLight::InitShader(Shader& _lightShader)
{
	//light direction from the fragment towards the light source
	Vectorf3 dirTowardSource = -(direction).Normalize();
	light.InitShader("directionalLight.", _lightShader);
	_lightShader.SetVec3("directionalLight.direction", dirTowardSource);
}
void SpotLight::InitShader(Shader& _lightShader)
{
	//light direction from the fragment towards the light source
	Vectorf3 dirTowardSource = -(direction).Normalize();
	point.InitShader(_lightShader, "spotLight.");
	_lightShader.SetVec3("spotLight.direction", dirTowardSource);
	float cutoffRad = cutoffDeg * M_PI / 180.f;
	_lightShader.SetFloat("spotLight.cutoff", cos(cutoffRad));
}