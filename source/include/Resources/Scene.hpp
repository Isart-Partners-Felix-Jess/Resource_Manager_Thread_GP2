#pragma once
#include <Camera.hpp>
#include <Light.hpp>

class Scene
{
public:
	Camera camera;
	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
		
	Scene(unsigned int _width, unsigned int _height);
	~Scene();
	void Init();
	void Update();
	void Destroy();
	void Texturetest();
private:
	//LearnOpenGl TODO: replace
	void Shadertest();
	void VBOtest();
	//void VBOCubetest();
	void VAOtest();
	void lightVAOtest();
	void EBOtest();
	void TransTest();
	void VBOCubetest();
	void LightTest();
};

