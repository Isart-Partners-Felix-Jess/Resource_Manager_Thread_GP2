#include <Scene.hpp>

#include <chrono>

Scene::Scene(unsigned int _width, unsigned int _height) : camera(_width, _height) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	m_justRestarted = true;
}

Scene::~Scene() {
	Destroy();
}

void Scene::Init()
{
	DEBUG_LOG(isMultiThreaded ? "\nMultithread\n" : "\nMonoThreaded\n");
	using namespace std::chrono;
	m_startLoad = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	m_orbInitDone = false;
	m_globalInitDone = false;
	m_materialsInitDone = false;
	//InitComponents
	models.resize(ModelName::size_model, nullptr);
	textures.resize(TextureName::size_texture, nullptr);

	if (isMultiThreaded)
		m_oneThreadToRuleThemAll = std::thread([this] { InitThread(); });
	InitShaders();
	InitLights();
	InitGraph();

	if (!isMultiThreaded)	// if Monothread
	{
		InitResources();
		InitMaterials();
		InitModels();
		//while (!ResourcesManager::IsPoolDone()) {};
		m_endLoad = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		m_durationLoad = m_endLoad - m_startLoad;
		Log::Print("Time total for loading: %u ms.", m_durationLoad);
		m_globalInitDone = true;
	}
	m_justRestarted = false;
}

void Scene::InitContinue()
{
	if (m_globalInitDone)
		return;
	static int frames = 0;
	if (++frames % 10)
		return;
	else if (!ResourcesManager::IsPoolDone())
	{
		InitResources();
		InitMaterials();
		InitModels();
	}
	else
	{
		//For the last Time
		InitResources();
		InitMaterials();
		InitModels();
		using namespace std::chrono;
		m_endLoad = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();	//	Maybe double for Monothreaded
		m_durationLoad = m_endLoad - m_startLoad;													//
		Log::Print("Time total for loading: %u ms.", m_durationLoad);							    //
		m_globalInitDone = true;
	}
}

void Scene::Update(const float& _deltaTime, const CameraInputs& _inputs)
{
	if (m_justRestarted)
		Init();
	else if (isMultiThreaded)
		InitContinue();
	camera.Update(_deltaTime, _inputs);

	static float time = 0.f;
	time += _deltaTime;
	float angularSpeed = 30; // In deg

	graph.entities[4]->SetTransform().rotation += Vectorf3{ 0.f,_deltaTime * angularSpeed, 0.f };
	graph.entities[4]->SetTransform().ComputeLocal();
	graph.entities[2]->SetTransform().translation = Vectorf3{ 0.f,0.5f + 0.5f * std::cos(time), -0.5f };
	graph.entities[2]->SetTransform().ComputeLocal();
	graph.Update(_deltaTime);

	// OpenGL test part
	UpdateLights(_deltaTime);
}

void Scene::Draw() {
	graph.Draw();
}

void Scene::Destroy()
{
	directionalLights.clear();
	pointLights.clear();
	spotLights.clear();
	if (m_oneThreadToRuleThemAll.joinable())
		m_oneThreadToRuleThemAll.join();
}

void Scene::Restart()
{
	if (!m_globalInitDone)
		return;
	ResourcesManager::Destroy();
	Scene::Destroy();
	graph.Destroy();

	isMultiThreaded = !isMultiThreaded; // Change the option multiThread<->monoThread

	models.clear();
	textures.clear();
	m_justRestarted = true;
}

void Scene::InitThread()
{
	ResourcesManager::CreateResourceThreaded<Texture>("white.png");
	ResourcesManager::CreateResourceThreaded<Model>("Horse");
	ResourcesManager::CreateResourceThreaded<Model>("viking_room");
	ResourcesManager::CreateResourceThreaded<Texture>("viking_room.jpg");
	ResourcesManager::CreateResourceThreaded<Model>("robot_operator");
	ResourcesManager::CreateResourceThreaded<Texture>("robot/base.png");
	ResourcesManager::CreateResourceThreaded<Texture>("robot/roughness.png");
	ResourcesManager::CreateResourceThreaded<Model>("cube");
	ResourcesManager::CreateResourceThreaded<Model>("objBuilding");
	ResourcesManager::CreateResourceThreaded<Texture>("objBuilding/brck91L.jpg");
	ResourcesManager::CreateResourceThreaded<Texture>("objBuilding/brck91Lb.jpg");
}

void Scene::InitResources()
{
	// The glorious, all important WHITE
	if (!textures[white_t])
		textures[white_t] = ResourcesManager::CreateResource<Texture>("white.png", isMultiThreaded);

	// Viking Room [0]
	if (!models[viking_room_m])
		models[viking_room_m] = ResourcesManager::CreateResource<Model>("viking_room", isMultiThreaded);
	// Get Viking Room texture
	if (!textures[viking_room_t])
		textures[viking_room_t] = ResourcesManager::CreateResource<Texture>("viking_room.jpg", isMultiThreaded);

	// Robot [1]
	if (!models[robot_m])
		models[robot_m] = ResourcesManager::CreateResource<Model>("robot_operator", isMultiThreaded);
	// Get Robot texture
	if (!textures[robot_base_t])
		textures[robot_base_t] = ResourcesManager::CreateResource<Texture>("robot/base.png", isMultiThreaded);
	// Get Robot lighting texture
	if (!textures[robot_roughness_t])
		textures[robot_roughness_t] = ResourcesManager::CreateResource<Texture>("robot/roughness.png", isMultiThreaded);

	// Copper Cube [2]
	if (!models[cube_m])
		models[cube_m] = ResourcesManager::CreateResource<Model>("cube", isMultiThreaded);

	// Building [3]
	if (!models[building_m])
		models[building_m] = ResourcesManager::CreateResource<Model>("objBuilding", isMultiThreaded);
	// Create texture in RManager
	if (!textures[objBuilding_brck91L_t])
		textures[objBuilding_brck91L_t] = ResourcesManager::CreateResource<Texture>("objBuilding/brck91L.jpg", isMultiThreaded);
	// Create texture in RManager
	if (!textures[objBuilding_brck91Lb_t])
		textures[objBuilding_brck91Lb_t] = ResourcesManager::CreateResource<Texture>("objBuilding/brck91Lb.jpg", isMultiThreaded);

	// LOOK AT MY HORSE [8]
	if (!models[horse_m])
			models[horse_m] = ResourcesManager::CreateResource<Model>("Horse", isMultiThreaded);
}

void Scene::InitLights()
{
	directionalLights.push_back({ Vectorf3{0.f,0.f,-1.f} });
	directionalLights.push_back(DirectionalLight{ Vectorf3{0.f,-1.f,0.f},
		{Vectorf3(.2f, 0.0f, 0.0f),
		Vectorf3(0.f, 0.5f, .0f),
		Vectorf3(0.8f, 0.8f, 1.0f) } });

	pointLights.push_back({ Vectorf3(-5.5f, 1.0f, 2.0f) });
	pointLights.push_back({ Vectorf3(-4.5f, 1.0f, 2.0f) });
	pointLights.push_back({ Vectorf3(-3.5f, 1.0f, 2.0f) });
	pointLights.push_back({ Vectorf3(-2.2f, 1.0f, 2.0f) });

	spotLights.push_back({ Vectorf3(0.f,0.f,-1.f),12.5f,17.5f, { {0.f,0.f,3.f},1.f,0.0f,0.032f} });
	spotLights.push_back({ Vectorf3(0.f,0.f,-1.f),12.5f,17.5f, { {0.f,0.f,3.f},1.f,0.0f,0.032f} });

	// In our case spotLight0 and 1 are torch lights (from the player), such as car headlights
	Vectorf3 xOffset = camera.up.Cross_product(camera.zCamera).Normalize() * 0.25f;
	spotLights[0].point.position = camera.eye - xOffset;
	spotLights[0].direction = camera.zCamera;
	spotLights[1].point.position = camera.eye + xOffset;
	spotLights[1].direction = camera.zCamera;
}

void Scene::UpdateLights(const float& _deltaTime)
{
	// "headlights" Spotlights
	Vectorf3 xOffset = camera.up.Cross_product(camera.zCamera).Normalize() * 0.25f;
	spotLights[0].point.position = camera.eye - xOffset;
	spotLights[0].direction = camera.zCamera;
	spotLights[1].point.position = camera.eye + xOffset;
	spotLights[1].direction = camera.zCamera;

	// Rotative Pointlight
	pointLights[0].position = matrix::Rotate3D(_deltaTime, matrix::Axis::Y) * pointLights[0].position;

	shadLight->Use();
	shadLight->SetInt("DIRECTIONAL_LIGHT_NBR", (int)directionalLights.size());
	for (unsigned int i = 0; i < directionalLights.size(); i++)
		directionalLights[i].InitShader(*shadLight, i);

	shadLight->SetInt("POINT_LIGHT_NBR", (int)pointLights.size());
	for (unsigned int i = 0; i < pointLights.size(); i++)
		pointLights[i].InitShader(*shadLight, i);

	shadLight->SetInt("SPOT_LIGHT_NBR", (int)spotLights.size());
	for (unsigned int i = 0; i < spotLights.size(); i++)
		spotLights[i].InitShader(*shadLight, i);

	shadLight->SetVec3("objectColor", 1.0f, 1.0f, 1.0f);
	shadLight->SetVec3("viewPos", camera.eye);
}

void Scene::InitGraph()
{
	// Viking Room [0]
	graph.AddEntity(nullptr, nullptr, Transform({ -4.f,0.f,0.f }, { 90.f,90.f,0.f }, { 3.f,3.f,3.f }));
	// Robot [1]
	graph.AddEntity(nullptr, nullptr, Transform({}, { 0.f,90.f,0.f }));
	// Copper Cube [2]
	graph.AddEntity(nullptr, nullptr, Transform({ 0.f,1.f,-0.5f }));
	// Building [3]
	graph.AddEntity(nullptr, nullptr, Transform({ 5.f,0.f, 0.f }, {}, { 0.1f,0.2f, 0.3f }));
	// Orbit [4]
	graph.AddEntity(nullptr, nullptr, Transform({ -7.f,0.f,0.f }));
	graph.entities[orbit_e]->AddChild(graph.entities[robot_e], true);
	// Orbs
	// Orb [5]
	graph.AddEntity(nullptr, graph.entities[robot_e], Transform({ 0.f,1.f,1.f }, {}, { 0.15f,0.15f,0.15f }));
	// Orb [6]
	graph.AddEntity(nullptr, graph.entities[robot_e], Transform({ 0.f,1.2f,0.0f }, {}, { 0.15f,0.15f,0.15f }));
	// Orb [7]
	graph.AddEntity(nullptr, graph.entities[robot_e], Transform({ 0.f,1.f,-1.f }, {}, { 0.15f,0.15f,0.15f }));
	// LOOK AT MY HORSE [8]
	graph.AddEntity(nullptr, nullptr, Transform({ 0.f,-1.f,10.f }, { 90.f,90.f,0.f }, { .3f, .3f, .3f }));
}

void Scene::InitModels()
{
	// Viking Room [0]
	if (models[viking_room_m] && models[viking_room_m]->IsReadFinished())
	{
		models[viking_room_m]->ResourceLoadOpenGL("viking_room");
		graph.entities[0]->model = models[viking_room_m];
	}
	// Set Viking Room texture
	if (textures[viking_room_t] && textures[viking_room_t]->IsReadFinished())
	{
		textures[viking_room_t]->ResourceLoadOpenGL("viking_room.jpg");
		graph.entities[viking_room_e]->material.AttachDiffuseMap(textures[viking_room_t]);
		graph.entities[viking_room_e]->material.AttachSpecularMap(textures[viking_room_t]);
	}

	// Robot [1]
	if (models[robot_m] && models[robot_m]->IsReadFinished())
	{
		models[robot_m]->ResourceLoadOpenGL("robot_operator");
		graph.entities[robot_e]->model = models[robot_m];
	}
	// Set Robot texture
	if (textures[robot_base_t] && textures[robot_base_t]->IsReadFinished())
	{
		textures[robot_base_t]->ResourceLoadOpenGL("robot/base.png");
		graph.entities[robot_e]->material.AttachDiffuseMap(textures[robot_base_t]);
	}
	// Set Robot lighting texture
	if (textures[robot_roughness_t] && textures[robot_roughness_t]->IsReadFinished())
	{
		textures[robot_roughness_t]->ResourceLoadOpenGL("robot/roughness.png");
		graph.entities[robot_e]->material.AttachSpecularMap(textures[robot_roughness_t]);
	}

	// Copper Cube [2]
	if (models[cube_m] && models[cube_m]->IsReadFinished())
	{
		models[cube_m]->ResourceLoadOpenGL("cube");
		graph.entities[copper_cube_e]->model = graph.entities[orb1_e]->model = graph.entities[orb2_e]->model = graph.entities[orb3_e]->model = models[cube_m];
		graph.entities[copper_cube_e]->material = material::copper;
		graph.entities[copper_cube_e]->material.AttachDiffuseMap(textures[white_t]);
		graph.entities[copper_cube_e]->material.AttachSpecularMap(textures[white_t]);
		graph.entities[copper_cube_e]->SetParent(graph.entities[robot_e]);
	}

	// Building [3]
	if (models[building_m] && models[building_m]->IsReadFinished())
	{
		models[building_m]->ResourceLoadOpenGL("objBuilding");
		graph.entities[building_e]->model = models[building_m];
	}

	// Bind texture to entity
	if (textures[objBuilding_brck91L_t] && textures[objBuilding_brck91L_t]->IsReadFinished())
	{
		textures[objBuilding_brck91L_t]->ResourceLoadOpenGL("objBuilding/brck91L.jpg");
		graph.entities[building_e]->material.AttachDiffuseMap(textures[objBuilding_brck91L_t]);
	}

	// Bind texture to entity
	if (textures[objBuilding_brck91Lb_t] && textures[objBuilding_brck91Lb_t]->IsReadFinished())
	{
		textures[objBuilding_brck91Lb_t]->ResourceLoadOpenGL("objBuilding/brck91Lb.jpg");
		graph.entities[building_e]->material.AttachSpecularMap(textures[objBuilding_brck91Lb_t]);
	}

	// Orbs
	if (!m_orbInitDone && textures[white_t])
	{
		// Orb1 [5]
		graph.entities[orb1_e]->material = material::ruby;
		graph.entities[orb1_e]->material.AttachDiffuseMap(textures[white_t]);
		graph.entities[orb1_e]->material.AttachSpecularMap(textures[white_t]);

		// Orb2 [6]
		graph.entities[orb2_e]->material = material::emerald;
		graph.entities[orb2_e]->material.AttachDiffuseMap(textures[white_t]);
		graph.entities[orb2_e]->material.AttachSpecularMap(textures[white_t]);

		// Orb3 [7]
		graph.entities[orb3_e]->material = material::turquoise;
		graph.entities[orb3_e]->material.AttachDiffuseMap(textures[white_t]);
		graph.entities[orb3_e]->material.AttachSpecularMap(textures[white_t]);

		m_orbInitDone = true;
	}

	// LOOK AT MY HORSE [8]
	if (models[horse_m] && models[horse_m]->IsReadFinished() && textures[white_t])
	{
		models[horse_m]->ResourceLoadOpenGL("Horse");
		graph.entities[horse_e]->model = models[horse_m];
		models[horse_m]->shader = shadLightCube;
		graph.entities[horse_e]->material = material::gold;
		graph.entities[horse_e]->material.AttachDiffuseMap(textures[white_t]);
		graph.entities[horse_e]->material.AttachSpecularMap(textures[white_t]);
	}

	// Do this last
	graph.InitDefaultShader(*shadLight);
}

void Scene::InitShaders()
{
	shadLight = ResourcesManager::CreateResource<Shader>("shadLight", false);
	shadLightCube = ResourcesManager::CreateResource<Shader>("shadLightCube", false);

	shadLight->ReadFragmentShader("assets/shaders/lighting.frag");
	shadLight->ReadVertexShader("assets/shaders/basic.vert");

	shadLight->SetFragmentShader();
	shadLight->SetVertexShader();

	shadLight->Link();

	shadLightCube->ReadFragmentShader("assets/shaders/white.frag");
	shadLightCube->ReadVertexShader("assets/shaders/basic.vert");

	shadLightCube->SetFragmentShader();
	shadLightCube->SetVertexShader();

	shadLightCube->Link();
}

void Scene::InitMaterials()
{
	// Only once and after white.png has been loaded
	if (m_materialsInitDone)
		return;

	if (textures[white_t]->IsReadFinished())
		textures[white_t]->ResourceLoadOpenGL("white.png");

	material::none.AttachDiffuseMap(textures[white_t]);
	material::none.AttachSpecularMap(textures[white_t]);

	for (unsigned int i = 0; i < 24; i++)
	{
		material::list[i].AttachDiffuseMap(textures[white_t]);
		material::list[i].AttachSpecularMap(textures[white_t]);
	}
	m_materialsInitDone = true;
}

//	The Dump
// Not used anymore, use it to test the demo materials
void Scene::MaterialTest()
{
	Vectorf3 cubePositions[24];
	for (size_t j = 0; j < 4; j++)
		for (size_t i = 0; i < 6; i++)
			cubePositions[j * 6 + i] = Vectorf3(2.f * i - +6, -2.f * j + 4, 0.0f);

	for (size_t i = 0; i < 24; i++)
	{
		float angle = 20.0f * i * static_cast<float>(M_PI_2) / 180;
		Matrix3x3 rotation = matrix::Rotate3dAllAxis(Vectorf3{ angle, .3f * angle, .5f * angle });
		Matrix4x4 modeltest = matrix::MatrixTRS(cubePositions[i].X(), cubePositions[i].Y(), cubePositions[i].Z(), angle, .3f * angle, .5f * angle, 1.f, 1.f, 1.f);

		// For model loader
		rotation = Matrix3x3(true);
		modeltest = Matrix4x4(true);

		Matrix4x4 MVP = camera.viewProjection * modeltest;

		shadLight->SetMat4("MVP", MVP);
		shadLight->SetMat4("model", modeltest);
		shadLight->SetMat3("normalMatrix", modeltest.Inversion().Transposed()); // It works :D
		models[cube_m]->ChangeMaterial(material::list[i], 0);
		models[cube_m]->Draw(*shadLight);

		material::none.InitShader(*shadLight);
	}
}
/*  models[building_m]->AddMaterials(8);
	models[building_m]->materials[1].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/wndw038M.jpg"));
	models[building_m]->materials[2].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/slat41XL.jpg"));
	models[building_m]->materials[2].AttachSpecularMap(ResourcesManager::CreateResource<Texture>("objBuilding/slat41XLb.jpg"));
	models[building_m]->materials[3].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/wndw107M.jpg"));
	models[building_m]->materials[3].AttachSpecularMap(ResourcesManager::CreateResource<Texture>("objBuilding/wndw107Mb.jpg"));
	models[building_m]->materials[5].AttachDiffuseMap(ResourcesManager::GetResource<Texture>("objBuilding/wndw038M.jpg"));
	models[building_m]->materials[5].AttachSpecularMap(ResourcesManager::GetResource<Texture>("objBuilding/wndw038M.jpg"));
	models[building_m]->materials[6].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/brck91L.jpg"));
	models[building_m]->materials[6].AttachSpecularMap(ResourcesManager::CreateResource<Texture>("objBuilding/brck91Lb.jpg"));
	models[building_m]->materials[7].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/ground009b.jpg"));
	models[building_m]->materials[7].AttachSpecularMap(ResourcesManager::GetResource<Texture>("objBuilding/ground009b.jpg"));
	models[building_m]->materials[8].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/germany010.jpg"));
	models[building_m]->materials[8].AttachSpecularMap(ResourcesManager::CreateResource<Texture>("objBuilding/germany010b.jpg")); */