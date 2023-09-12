#pragma once

#include<vectorM.hpp>

class Shader;

struct Light
{
	Vectorf3 ambient{ 0.2f,0.2f, 0.2f };
	Vectorf3 diffuse{ .5f, .5f, .5f };
	Vectorf3 specular{ 1.f,1.f, 1.f };
	float ambientStrength = 0.1f;	// Could go in last vector component,
	float specularStrength = 0.5f;	// I'd rather separate for clarity

	//Should go in Renderer
	void InitShader(std::string _lightType, Shader& _lightShader);
};

struct PointLight
{
	Vectorf3 position;
	// Attenuation
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	Light light{};

	void InitShader(Shader& _lightShader, unsigned int _number);
	//ONLY To init a point light inside of another structlight (such as Spotlight)
	void InitShader(Shader& _lightShader, std::string _structType);

private:
	void InitShaderKnownName(Shader& _lightShader, std::string _structType);
};

struct DirectionalLight
{
	Vectorf3 direction;
	Light light{};

	void InitShader(Shader& _lightShader, unsigned int _number);
};

struct SpotLight
{
	Vectorf3 direction;
	float cutoffDeg;
	float outerCutoffDeg;
	PointLight point;

	void InitShader(Shader& _lightShader, unsigned int _number);
};