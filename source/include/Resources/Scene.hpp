#pragma once

#include <Camera.hpp>
#include <Light.hpp>

#include <Graph.hpp>
#include <Shader.hpp>

#include <Model.hpp>
#include <ResourcesManager.hpp>

#include <Texture.hpp>
#include <Material.hpp>

#include <../Thread/ThreadPool.hpp>

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