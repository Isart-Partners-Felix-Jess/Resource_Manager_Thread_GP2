#include <Light.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include <Shader.hpp>

void Light::InitShader(std::string _lightType, Shader& _lightShader)
{
	_lightShader.SetVec3(_lightType + "light.ambientColor", ambient);
	_lightShader.SetVec3(_lightType + "light.diffuseColor", diffuse);
	_lightShader.SetVec3(_lightType + "light.specularColor", specular);
	_lightShader.SetFloat(_lightType + "light.ambientStrength", ambientStrength);
	_lightShader.SetFloat(_lightType + "light.specularStrength", specularStrength);
}

void PointLight::InitShader(Shader& _lightShader, unsigned int _number) {
	InitShaderKnownName(_lightShader, "pointLight[" + std::to_string(_number) + "].");
}

void PointLight::InitShader(Shader& _lightShader, std::string _structType) {
	InitShaderKnownName(_lightShader, _structType + "pointLight.");
}

void PointLight::InitShaderKnownName(Shader& _lightShader, std::string _fullName)
{
	light.InitShader(_fullName, _lightShader);
	_lightShader.SetVec3(_fullName + "position", position);
	_lightShader.SetFloat(_fullName + "constant", constant);
	_lightShader.SetFloat(_fullName + "linear", linear);
	_lightShader.SetFloat(_fullName + "quadratic", quadratic);
}

void DirectionalLight::InitShader(Shader& _lightShader, unsigned int _number)
{
	std::string nbstr = "directionalLight[" + std::to_string(_number) + "].";
	//light direction from the fragment towards the light source
	Vectorf3 dirTowardSource = -(direction).Normalize();
	light.InitShader(nbstr, _lightShader);
	_lightShader.SetVec3(nbstr + "direction", dirTowardSource);
}

void SpotLight::InitShader(Shader& _lightShader, unsigned int _number)
{
	std::string nbstr = "spotLight[" + std::to_string(_number) + "].";
	//light direction from the fragment towards the light source
	Vectorf3 dirTowardSource = -(direction).Normalize();
	point.InitShader(_lightShader, nbstr);
	_lightShader.SetVec3(nbstr + "direction", dirTowardSource);
	float deg2Rad = static_cast<float>(M_PI) / 180.f;
	float cutoffRad = cutoffDeg * deg2Rad;
	_lightShader.SetFloat(nbstr + "cutoff", cos(cutoffRad));
	float outerCutoffRad = outerCutoffDeg * deg2Rad;
	_lightShader.SetFloat(nbstr + "outerCutoff", cos(outerCutoffRad));
}