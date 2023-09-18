#include <Scene.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <ImGui/imgui.h>

void Scene::Init()
{
	//InitComponents
	if (!monoThreaded)
		InitThread();
	InitMaterials();

	InitLights();
	InitModels();
	InitShaders();
}

void Scene::Update(const float& _deltaTime, const CameraInputs& _inputs)
{
	camera.Update(_deltaTime, _inputs);

	static float time = 0.f;
	time += _deltaTime;
	float angularSpeed = 30; // In deg

	graph.entities[4]->SetTransform().rotation += Vectorf3{ 0.f,_deltaTime * angularSpeed,0.f };
	graph.entities[4]->SetTransform().ComputeLocal();
	graph.entities[2]->SetTransform().translation = Vectorf3{ 0.f,0.5f + 0.5f * std::cos(time),-0.5f };
	graph.entities[2]->SetTransform().ComputeLocal();
	graph.Update(_deltaTime);

	// OpenGL test part
	UpdateLights(_deltaTime);
}

void Scene::Destroy()
{
	directionalLights.clear();
	pointLights.clear();
	spotLights.clear();
	threadPool.clear();
}

void Scene::Restart()
{
	graph.Destroy();
	Scene::Destroy(); 
	ResourcesManager::Destroy();;
	monoThreaded = !monoThreaded; //Change the option multi<->mono

	Scene::Init();
}

void Scene::InitThread()
{
	threadPool.push_back(ResourcesManager::CreateResourceThread<Model>(std::string("viking_room")));
	threadPool.push_back(ResourcesManager::CreateResourceThread<Texture>("viking_room.jpg"));
	threadPool.push_back(ResourcesManager::CreateResourceThread<Model>(std::string("robot_operator")));
	threadPool.push_back(ResourcesManager::CreateResourceThread<Texture>("robot/base.png"));
	threadPool.push_back(ResourcesManager::CreateResourceThread<Texture>("robot/roughness.png"));
	threadPool.push_back(ResourcesManager::CreateResourceThread<Model>(std::string("cube")));
	threadPool.push_back(ResourcesManager::CreateResourceThread<Model>(std::string("objBuilding")));
	threadPool.push_back(ResourcesManager::CreateResourceThread<Texture>("objBuilding/brck91L.jpg"));
	threadPool.push_back(ResourcesManager::CreateResourceThread<Texture>("objBuilding/brck91Lb.jpg"));
	threadPool.push_back(ResourcesManager::CreateResourceThread<Texture>("white.png"));


	for (std::thread& t : threadPool)
		t.join();
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
	Vectorf3 X_Offset = camera.up.Cross_product(camera.zCamera).Normalize() * 0.25f;
	spotLights[0].point.position = camera.eye - X_Offset;
	spotLights[0].direction = camera.zCamera;
	spotLights[1].point.position = camera.eye + X_Offset;
	spotLights[1].direction = camera.zCamera;
}
void Scene::UpdateLights(const float& _deltaTime)
{
	// "headlights" Spotlights
	Vectorf3 X_Offset = camera.up.Cross_product(camera.zCamera).Normalize() * 0.25f;
	spotLights[0].point.position = camera.eye - X_Offset;
	spotLights[0].direction = camera.zCamera;
	spotLights[1].point.position = camera.eye + X_Offset;
	spotLights[1].direction = camera.zCamera;

	// Rotative Pointlight
	pointLights[0].position = matrix::Rotate3D(_deltaTime, matrix::Axis::Y) * pointLights[0].position;

	shadlight->Use();
	shadlight->SetInt("DIRECTIONAL_LIGHT_NBR", directionalLights.size());
	for (size_t i = 0; i < directionalLights.size(); i++)
		directionalLights[i].InitShader(*shadlight, i);

	shadlight->SetInt("POINT_LIGHT_NBR", pointLights.size());
	for (size_t i = 0; i < pointLights.size(); i++)
		pointLights[i].InitShader(*shadlight, i);

	shadlight->SetInt("SPOT_LIGHT_NBR", (int)spotLights.size());
	for (size_t i = 0; i < spotLights.size(); i++)
		spotLights[i].InitShader(*shadlight, i);

	shadlight->SetVec3("objectColor", 1.0f, 1.0f, 1.0f);
	shadlight->SetVec3("viewPos", camera.eye);
}

void Scene::InitModels()
{
	//building->AddMaterials(8);
	//building->materials[1].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/wndw038M.jpg"));
	//building->materials[2].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/slat41XL.jpg"));
	//building->materials[2].AttachSpecularMap(ResourcesManager::CreateResource<Texture>("objBuilding/slat41XLb.jpg"));
	//building->materials[3].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/wndw107M.jpg"));
	//building->materials[3].AttachSpecularMap(ResourcesManager::CreateResource<Texture>("objBuilding/wndw107Mb.jpg"));
	//building->materials[5].AttachDiffuseMap(ResourcesManager::GetResource<Texture>("objBuilding/wndw038M.jpg"));
	//building->materials[5].AttachSpecularMap(ResourcesManager::GetResource<Texture>("objBuilding/wndw038M.jpg"));
	//building->materials[6].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/brck91L.jpg"));
	//building->materials[6].AttachSpecularMap(ResourcesManager::CreateResource<Texture>("objBuilding/brck91Lb.jpg"));
	//building->materials[7].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/ground009b.jpg"));
	//building->materials[7].AttachSpecularMap(ResourcesManager::GetResource<Texture>("objBuilding/ground009b.jpg"));
	//building->materials[8].AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/germany010.jpg"));
	//building->materials[8].AttachSpecularMap(ResourcesManager::CreateResource<Texture>("objBuilding/germany010b.jpg"));

	// Viking Room [0]
	if (monoThreaded)
		viking_room = ResourcesManager::CreateResource<Model>(std::string("viking_room"));
	else
	{
		viking_room = ResourcesManager::CreateResourceThreadJoined<Model>(std::string("viking_room"));
	}
	graph.AddEntity(viking_room, nullptr, Transform({ -4.f,0.f,0.f }, { 90.f,90.f,0.f }, { 3.f,3.f,3.f }));
	if (monoThreaded)
		graph.entities[0]->material.AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("viking_room.jpg"));
	else
		graph.entities[0]->material.AttachDiffuseMap(ResourcesManager::CreateResourceThreadJoined<Texture>("viking_room.jpg"));
	graph.entities[0]->material.AttachSpecularMap(ResourcesManager::GetResource<Texture>("viking_room.jpg"));

	// Robot [1]
	if (monoThreaded)
		robot = ResourcesManager::CreateResource<Model>(std::string("robot_operator"));
	else
		robot = ResourcesManager::CreateResourceThreadJoined<Model>(std::string("robot_operator"));
	graph.AddEntity(robot, nullptr, Transform({}, { 0.f,90.f,0.f }));
	if (monoThreaded)
	{
		graph.entities[1]->material.AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("robot/base.png"));
		graph.entities[1]->material.AttachSpecularMap(ResourcesManager::CreateResource<Texture>("robot/roughness.png"));
	}
	else
	{
		graph.entities[1]->material.AttachDiffuseMap(ResourcesManager::CreateResourceThreadJoined<Texture>("robot/base.png"));
		graph.entities[1]->material.AttachSpecularMap(ResourcesManager::CreateResourceThreadJoined<Texture>("robot/roughness.png"));
	}
	// Copper Box [2]
	if (monoThreaded)
		cube = ResourcesManager::CreateResource<Model>(std::string("cube"));
	else
		cube = ResourcesManager::CreateResourceThreadJoined<Model>(std::string("cube"));
	graph.AddEntity(cube, nullptr, Transform({ 0.f,1.f,-0.5f }));
	graph.entities[2]->material = material::copper;
	graph.entities[2]->material.AttachDiffuseMap(ResourcesManager::GetResource<Texture>("white.png"));
	graph.entities[2]->material.AttachSpecularMap(ResourcesManager::GetResource<Texture>("white.png"));
	graph.entities[2]->SetParent(graph.entities[1]);

	// Building [3]
	if (monoThreaded)
		building = ResourcesManager::CreateResource<Model>(std::string("objBuilding"));
	else
		building = ResourcesManager::CreateResourceThreadJoined<Model>(std::string("objBuilding"));
	graph.AddEntity(building, nullptr, Transform({ 5.f,0.f, 0.f }, {}, { 0.1f,0.2f, 0.3f }));

	if (monoThreaded)
	{
		graph.entities[3]->material.AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("objBuilding/brck91L.jpg"));
		graph.entities[3]->material.AttachSpecularMap(ResourcesManager::CreateResource<Texture>("objBuilding/brck91Lb.jpg"));
	}
	else
	{
		graph.entities[3]->material.AttachDiffuseMap(ResourcesManager::CreateResourceThreadJoined<Texture>("objBuilding/brck91L.jpg"));
		graph.entities[3]->material.AttachSpecularMap(ResourcesManager::CreateResourceThreadJoined<Texture>("objBuilding/brck91Lb.jpg"));
	}
	// Orbit [4]
	graph.AddEntity(nullptr, nullptr, Transform({ -7.f,0.f,0.f }));
	graph.entities[4]->AddChild(graph.entities[1], true);

	// Orbs
	// Orb [5]
	graph.AddEntity(cube, graph.entities[1], Transform({ 0.f,1.f,1.f }, {}, { 0.15f,0.15f,0.15f }));
	graph.entities[5]->material = material::ruby;
	graph.entities[5]->material.AttachDiffuseMap(ResourcesManager::GetResource<Texture>("white.png"));
	graph.entities[5]->material.AttachSpecularMap(ResourcesManager::GetResource<Texture>("white.png"));

	// Orb [6]
	graph.AddEntity(cube, graph.entities[1], Transform({ 0.f,1.2f,0.0f }, {}, { 0.15f,0.15f,0.15f }));
	graph.entities[6]->material = material::emerald;
	graph.entities[6]->material.AttachDiffuseMap(ResourcesManager::GetResource<Texture>("white.png"));
	graph.entities[6]->material.AttachSpecularMap(ResourcesManager::GetResource<Texture>("white.png"));

	// Orb [7]
	graph.AddEntity(cube, graph.entities[1], Transform({ 0.f,1.f,-1.f }, {}, { 0.15f,0.15f,0.15f }));
	graph.entities[7]->material = material::turquoise;
	graph.entities[7]->material.AttachDiffuseMap(ResourcesManager::GetResource<Texture>("white.png"));
	graph.entities[7]->material.AttachSpecularMap(ResourcesManager::GetResource<Texture>("white.png"));
	graph.InitDefaultShader(*shadlight);
}

void Scene::InitShaders()
{
	shadlight = ResourcesManager::CreateResource<Shader>("shadlight");
	shadlightCube = ResourcesManager::CreateResource<Shader>("shadlightCube");

	shadlight->SetFragmentShader("assets/shaders/lighting.frag");
	shadlight->SetVertexShader("assets/shaders/basic.vert");
	shadlight->Link();

	shadlightCube->SetFragmentShader("assets/shaders/white.frag");
	shadlightCube->SetVertexShader("assets/shaders/basic.vert");
	shadlightCube->Link();
	graph.InitDefaultShader(*shadlight);
}

void Scene::InitMaterials()
{
	if (monoThreaded)
		material::none.AttachDiffuseMap(ResourcesManager::CreateResource<Texture>("white.png"));
	else
		material::none.AttachDiffuseMap(ResourcesManager::CreateResourceThreadJoined<Texture>("white.png"));

	material::none.AttachSpecularMap(ResourcesManager::GetResource<Texture>("white.png"));
	for (unsigned int i = 0; i < 24; i++)
	{
		material::list[i].AttachDiffuseMap(ResourcesManager::GetResource<Texture>("white.png"));
		material::list[i].AttachSpecularMap(ResourcesManager::GetResource<Texture>("white.png"));
	}
}

// Not used anymore, use it to test the demo materials
void Scene::MaterialTest()
{
	Vectorf3 cubePositions[24];
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 6; i++)
			cubePositions[j * 6 + i] = Vectorf3(2.f * i - +6, -2.f * j + 4, 0.0f);

	for (unsigned int i = 0; i < 24; i++)
	{
		float angle = 20.0f * i * static_cast<float>(M_PI_2) / 180;
		Matrix3x3 rotation = matrix::Rotate3dAllAxis(Vectorf3{ angle, .3f * angle, .5f * angle });
		Matrix4x4 modeltest = matrix::MatrixTRS(cubePositions[i].X(), cubePositions[i].Y(), cubePositions[i].Z(), angle, .3f * angle, .5f * angle, 1.f, 1.f, 1.f);

		// For model loader
		rotation = Matrix3x3(true);
		modeltest = Matrix4x4(true);

		Matrix4x4 MVP = camera.viewProjection * modeltest;

		shadlight->SetMat4("MVP", MVP);
		shadlight->SetMat4("model", modeltest);
		shadlight->SetMat3("normalMatrix", modeltest.Inversion().Transposed()); // It works :D
		cube->ChangeMaterial(material::list[i], 0);
		cube->Draw(*shadlight);

		material::none.InitShader(*shadlight);
	}
}