#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include<vectorM.hpp>

struct Light
{
	Vectorf3 position;
	Vectorf3 ambient{1.f,1.f, 1.f};
	Vectorf3 diffuse{ 1.f,1.f, 1.f };
	Vectorf3 specular{ 1.f,1.f, 1.f };
	float ambientStrength = 0.1; // Could go in last parameter,
	float specularStrength = 0.5;// I'd rather separate for clarity
};
#endif // !LIGHT_H

