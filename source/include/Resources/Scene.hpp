#pragma once

#include <Camera.hpp>
#include <Light.hpp>

#include <Graph.hpp>
#include <Shader.hpp>

#include <Model.hpp>
#include <ResourcesManager.hpp>

#include <Texture.hpp>
#include <Material.hpp>

class Scene
{
public:
	Camera camera;
	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;

	SceneGraph graph = SceneGraph(this);
	std::vector<std::thread> threadPool;

	bool monoThreaded = true;

	Shader& shadlight;
	Shader& shadlightCube;

	std::vector<Model*> lightCubes;
	Model* cube = nullptr;
	Model* viking_room = nullptr;
	Model* robot = nullptr;
	Model* building = nullptr;

	Scene(unsigned int _width, unsigned int _height) :
		camera(_width, _height),
		shadlight(*ResourcesManager::CreateResource<Shader>("shadlight")),
		shadlightCube(*ResourcesManager::CreateResource<Shader>("shadlightCube"))
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
	~Scene() {
		Destroy();
	}

	void Init();
	void Update(const float& _deltaTime, const CameraInputs& _inputs);
	void Draw() {
		graph.Draw();
	}
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