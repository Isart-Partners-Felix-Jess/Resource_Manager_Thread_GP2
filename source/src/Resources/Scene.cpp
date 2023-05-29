#include <Scene.hpp>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <ImGui/imgui.h>

#include <ResourcesManager.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Material.hpp>
#include <Model.hpp>


//TODO: DELETE THIS !!!!!!
unsigned int EBO;
unsigned int lightVAO;
unsigned int VAO;
unsigned int VBO;
unsigned int VBO2;
Shader shadlight;
Shader shadlightCube;

Scene::Scene(unsigned int _width, unsigned int _height) : camera(_width, _height)
{
}
Scene::~Scene()
{
	Destroy();
}
void Scene::Init()
{
	directionalLights.push_back({ Vectorf3{0.f,0.f,-1.f}
		/*Vectorf3(1.2f, 1.0f, 2.0f),
		Vectorf3(1.0f, 0.0f, 0.0f),
		Vectorf3(.0f, 1.0f, .0f),
		Vectorf3(0.0f, 0.0f, 1.0f) */ });
	pointLights.push_back({ Vectorf3(1.2f, 1.0f, 2.0f) });
	pointLights.push_back({ Vectorf3(2.2f, 1.0f, 2.0f) });
	pointLights.push_back({ Vectorf3(3.2f, 1.0f, 2.0f) });
	pointLights.push_back({ Vectorf3(4.2f, 1.0f, 2.0f) });
	spotLights.push_back({ Vectorf3(0.f,0.f,-1.f),12.5f,17.5f, { {0.f,0.f,3.f},1.f,0.0f,0.032f} });
	//LearnOpenGL tuto
	Texturetest();
	Shadertest();
	//TransTest();
}

void Scene::Update()
{
	//OpenGL test part
//int vertexColorLocation = glGetUniformLocation(shadbasic.GetShaderProgram(), "ourColor");
	LightTest();
	//shadbasic.Use();//Only if forgotten before
	//shadbasic.SetInt("texture1", 0);
	//shadbasic.SetInt("texture2", 1);
	//shadbasic.SetFloat("mixValue", mixValue);
	TransTest();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//shadlightCube.Use();
	//glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//shadlight.Use();
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	//
}

void Scene::Destroy()
{
	directionalLights.clear();
	pointLights.clear();
	spotLights.clear();
	//LearnOpenGL Tuto
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shadlight.GetShaderProgram());
}
void Scene::Shadertest()
{
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};
	lightVAOtest();
	EBOtest();
	//VBOtest();
	VBOCubetest();
	shadlight.SetFragmentShader("assets/shaders/lighting.frag");
	shadlight.SetVertexShader("assets/shaders/basic.vert");
	shadlight.Link();
	shadlightCube.SetFragmentShader("assets/shaders/white.frag");
	shadlightCube.SetVertexShader("assets/shaders/basic.vert");
	shadlightCube.Link();

	//Render part
	//shadlight.Use();
	//glBindVertexArray(VAO);

}
//ElementsBufferOutput
void Scene::EBOtest()
{
	unsigned int indices[] = {  // note that we start from 0!
0, 1, 3,   // first triangle
1, 2, 3    // second triangle
	};
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
//VertexBufferOutput
void Scene::VBOtest()
{
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}
//VertexArrayOutput
void Scene::VAOtest()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

}
void Scene::lightVAOtest()
{
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//// set the vertex attribute 
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
}
void Scene::Texturetest()
{
	//ResourcesManager::CreateResource<Texture>(std::string("container.jpg"));
	//ResourcesManager::CreateResource<Texture>(std::string("awesomeface.png"));
	//ResourcesManager::CreateResource<Texture>(std::string("container2.png"));
	//ResourcesManager::CreateResource<Texture>(std::string("container2_specular.png"));


	Texture& container = *ResourcesManager::CreateResource<Texture>("container.jpg");
	Texture& awesomeface = *ResourcesManager::CreateResource<Texture>("awesomeface.png");
	Texture& container2 = *ResourcesManager::CreateResource<Texture>("container2.png");
	Texture& container2_specular = *ResourcesManager::CreateResource<Texture>("container2_specular.png");
	//Texture container("container.jpg");
	//Texture awesomeface("awesomeface.png");
	//Texture container2("container2.png");
	//Texture container2_specular("container2_specular.png");
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, container.GetID());
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, awesomeface.GetID());
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, container2.GetID());
	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, container2_specular.GetID());
}

void Scene::TransTest()
{
	//Vectorf3 cubePositions[] = {
	//Vectorf3(0.0f,  0.0f,  0.0f),
	//Vectorf3(2.0f,  5.0f, -15.0f),
	//Vectorf3(-1.5f, -2.2f, -2.5f),
	//Vectorf3(-3.8f, -2.0f, -12.3f),
	//Vectorf3(2.4f, -0.4f, -3.5f),
	//Vectorf3(-1.7f,  3.0f, -7.5f),
	//Vectorf3(1.3f, -2.0f, -2.5f),
	//Vectorf3(1.5f,  2.0f, -2.5f),
	//Vectorf3(1.5f,  0.2f, -1.5f),
	//Vectorf3(-1.3f,  1.0f, -1.5f)
	//};
	Vectorf3 cubePositions[24];
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 6; i++)
			cubePositions[j * 6 + i] = Vectorf3(2.f * i - +6, -2.f * j + 4, 0.0f);
	for (unsigned int i = 0; i < 24; i++)
	{
		float angle = 20.0f * i * static_cast<float>(M_PI_2) / 180;
		Matrix3x3 rotation = matrix::Rotate3dAllAxis(Vectorf3{ angle, .3f * angle, .5f * angle });
		Matrix4x4 modeltest = matrix::MatrixTRS(cubePositions[i].X(), cubePositions[i].Y(), cubePositions[i].Z(), angle, .3f * angle, .5f * angle, 1.f, 1.f, 1.f);

		Matrix4x4 MVP = camera.viewProjection * modeltest;

		shadlight.SetMat4("MVP", MVP);
		shadlight.SetMat4("model", modeltest);
		//shadlight.SetMat3("normalMatrix", modeltest.Inversion().Transposed()); //It works :D
		shadlight.SetMat3("normalMatrix", rotation);
		material::list[i].AttachDiffuseMap(2);
		material::list[i].AttachSpecularMap(3);
		//material::none.InitShader(shadlight);
		material::list[i].InitShader(shadlight);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
void Scene::LightTest()
{
	shadlight.SetInt("DIRECTIONAL_LIGHTS_NBR", directionalLights.size());
	shadlight.SetInt("POINT_LIGHTS_NBR", pointLights.size());
	shadlight.SetInt("SPOT_LIGHTS_NBR", spotLights.size());
	spotLights[0].point.position = camera.eye;
	spotLights[0].direction = camera.zCamera;
	pointLights[0].position = matrix::Rotate3D(ImGui::GetIO().DeltaTime, matrix::Axis::Y) * pointLights[0].position;
	shadlightCube.Use();
	Matrix4x4 model = matrix::MatrixTRS(pointLights[0].position, {}, { 1.f,1.f,1.f });
	model = model * 0.2f;
	Matrix4x4 MVP = camera.viewProjection * model;
	unsigned int MVPLoc = glGetUniformLocation(shadlightCube.GetShaderProgram(), "MVP");
	shadlightCube.SetMat4("MVP", MVP);
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shadlight.Use();
	shadlight.SetVec3("objectColor", 1.0f, 1.0f, 1.0f/* 0.5f, 0.31f*/);
	pointLights[0].InitShader(shadlight, 0);
	pointLights[1].InitShader(shadlight, 1);
	pointLights[2].InitShader(shadlight, 2);
	pointLights[3].InitShader(shadlight, 3);
	directionalLights[0].InitShader(shadlight, 0);
	spotLights[0].InitShader(shadlight, 0);
	shadlight.SetVec3("viewPos", camera.eye);
}

//VertexBufferOutput
void Scene::VBOCubetest()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f,  0.0f
	};
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// normal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}