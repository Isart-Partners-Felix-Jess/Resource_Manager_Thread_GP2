#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include<vectorM.hpp>

class Shader;
struct Light
{
	Vectorf3 ambient{1.f,1.f, 1.f};
	Vectorf3 diffuse{ 1.f,1.f, 1.f };
	Vectorf3 specular{ 1.f,1.f, 1.f };
	float ambientStrength = 0.1; // Could go in last parameter,
	float specularStrength = 0.5;// I'd rather separate for clarity

	//Should go in Renderer
	void InitShader(std::string _lightType,Shader& _lightShader);
};
struct PointLight
{
	Vectorf3 position;
	//Atenuation
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	Light light{};
	void InitShader(Shader& _lightShader);
};
struct DirectionalLight
{
	Vectorf3 direction;
	Light light{};

	void InitShader(Shader& _lightShader);
};
#endif // !LIGHT_H

