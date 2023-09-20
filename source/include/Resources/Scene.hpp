#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <Camera.hpp>
#include <Light.hpp>

#include <Graph.hpp>
#include <ResourcesManager.hpp>

enum ModelName
{
	viking_room_m,
	robot_m,
	cube_m,
	building_m,
	horse_m,

	size_model
};

enum TextureName
{
	white_t,
	viking_room_t,
	robot_base_t,
	robot_roughness_t,
	objBuilding_brck91L_t,
	objBuilding_brck91Lb_t,

	size_texture
};

enum EntityName
{
	viking_room_e,
	robot_e,
	copper_cube_e,
	building_e,
	orbit_e,
	orb1_e,
	orb2_e,
	orb3_e,
	horse_e,

	size_entity
};

class Scene
{
public:
	Camera camera;
	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;

	SceneGraph graph = SceneGraph(this);

	bool isMultiThreaded = true;

	Shader* shadLight = nullptr;
	Shader* shadLightCube = nullptr;
	
	std::vector<Model*> models;
	std::vector<Texture*> textures;

	// For tests
	std::vector<Model*> lightCubes;

	Scene(unsigned int _width, unsigned int _height);
	~Scene();

	void Init();
	void InitContinue();
	void Update(const float& _deltaTime, const CameraInputs& _inputs);
	void Draw();
	void Destroy();
	void Restart();

private:
	bool m_justRestarted = true;

	std::thread m_oneThreadToRuleThemAll;
	uint64_t m_startLoad = 0;
	uint64_t m_endLoad = 0;
	uint64_t m_durationLoad = 0;
	
#pragma region Init
	bool m_orbInitDone = false;
	bool m_globalInitDone = false;
	bool m_materialsInitDone = false;

	void InitThread();
	void InitResources();
	void InitLights();
	void InitGraph();
	void InitModels();
	void InitMaterials();
	void InitShaders();
#pragma endregion //Init

	void UpdateLights(const float& _deltaTime);

	void MaterialTest();
};