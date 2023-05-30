#pragma once
#include <Camera.hpp>
#include <Light.hpp>
#include <Graph.hpp>

//Temp
class Model;

class Scene
{
public:
	Camera camera;
	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	
	Graph graph;

	//Temp
	Model* viking_room;

	Scene(unsigned int _width, unsigned int _height);
	~Scene();
	void Init();
	void Update();
	void Destroy();
	void Texturetest();
private:
	void InitLights();
	void UpdateLights();

	//LearnOpenGl TODO: replace
	void InitModeltest();
	void DrawModeltest();

	void Shadertest();
	void VBOtest();
	//void VBOCubetest();
	void VAOtest();
	void lightVAOtest();
	void EBOtest();
	void TransTest();
	void VBOCubetest();
};

