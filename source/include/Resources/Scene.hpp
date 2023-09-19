#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <Camera.hpp>
#include <Light.hpp>

#include <Graph.hpp>

#include <ResourcesManager.hpp>

class Scene
{
public:
	Camera camera;
	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;

	SceneGraph graph = SceneGraph(this);

	bool isMultiThreaded = true;

	Shader* shadlight = nullptr;
	Shader* shadlightCube = nullptr;

	std::vector<Model*> lightCubes;
	Model* cube = nullptr;
	Model* viking_room = nullptr;
	Model* robot = nullptr;
	Model* building = nullptr;
	Model* horse = nullptr;

	Scene(unsigned int _width, unsigned int _height);
	~Scene();

	void Init();
	void Update(const float& _deltaTime, const CameraInputs& _inputs);
	void Draw();
	void Destroy();
	void Restart();

private:
	void InitThread();
	void InitLights();
	void InitModels();
	void InitMaterials();
	void InitShaders();

	void UpdateLights(const float& _deltaTime);

	void MaterialTest();
};