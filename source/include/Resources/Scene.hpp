#pragma once
#include <Camera.hpp>
#include <Light.hpp>
#include <Graph.hpp>
#include <Shader.hpp>

//Temp
class Model;

class Scene
{
public:
	Camera camera;
	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	
	SceneGraph graph = SceneGraph(this);

	Shader& shadlight;
	Shader& shadlightCube;

	std::vector<Model*> lightCubes;
	Model* cube;
	Model* viking_room;
	Model* robot;
	Model* building;


	Scene(unsigned int _width, unsigned int _height);
	~Scene();
	void Init();
	void Update(const float& _deltaTime, const CameraInputs& _inputs);
	void Draw();
	void Destroy();
private:
	void InitLights();
	void InitModels();
	void InitMaterials();
	void InitShaders();
	void UpdateLights(const float& _deltaTime);

	void MaterialTest();
};

