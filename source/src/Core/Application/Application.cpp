#include <Application.hpp>
#include <iostream>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Material.hpp>
#include <Light.hpp>
#include <matrix.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

//TODO: DELETE THIS !!!!!!
unsigned int EBO;
unsigned int lightVAO;
unsigned int VAO;
unsigned int VBO;
unsigned int VBO2;
Shader shadbasic;
Shader shadlight;
Shader shadlightCube;
DirectionalLight dLight
{ Vectorf3{0.f,0.f,-1.f}
	/*Vectorf3(1.2f, 1.0f, 2.0f),
Vectorf3(1.0f, 0.0f, 0.0f),
Vectorf3(.0f, 1.0f, .0f),
Vectorf3(0.0f, 0.0f, 1.0f) */ };
PointLight pLight{ Vectorf3(1.2f, 1.0f, 2.0f) };
SpotLight sLight{ Vectorf3(0.f,0.f,-1.f),12.5f,17.5f, { {0.f,0.f,3.f},1.f,0.0f,0.032f} };

float mixValue = 0.2f;

//static declarations
float Application::s_MouseScrollOffset = 0.0f;

Application::Application() :Application(800, 600)
{
}

Application::Application(int _width, int _height) :camera(_width, _height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(_width, _height, "Pipeline", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		//Always assert I want the above  code to apply
		Assert(false, "Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);
	Assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");

	//LearnOpenGL tuto
	Texturetest();
	Shadertest();
	//TransTest();

	glViewport(0, 0, _width, _height);
	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	SetupImGui(window);
}

Application::~Application()
{
	//LearnOpenGL Tuto
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shadbasic.GetShaderProgram());
	glDeleteProgram(shadlight.GetShaderProgram());

	//IMGUI Destroy
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glDisable(GL_DEPTH_TEST);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Application::Update()
{
	float deltaTime = 0.f, lastFrame = 0.f;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		StartImGuiFrame();
		ProcessInput(window);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.Update(deltaTime, inputs);
		if (m_ShowControls)
			ShowImGuiControls();
		Render(window);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
}

void Application::ApplyChangeColor()
{
	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	DEBUG_LOG("New Clear Color : %i,%i,%i", (int)(255.f * m_ClearColor[0]), (int)(255.f * m_ClearColor[1]), (int)(255.f * m_ClearColor[2]));
}
void Application::Shadertest()
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
	shadbasic.LoadResource("assets/shaders/basic");
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
void Application::EBOtest()
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
void Application::VBOtest()
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
void Application::VAOtest()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

}
void Application::lightVAOtest()
{
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//// set the vertex attribute 
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
}
void Application::Texturetest()
{
	Texture container("container.jpg");
	Texture awesomeface("awesomeface.png");
	Texture container2("container2.png");
	Texture container2_specular("container2_specular.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, container.GetID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, awesomeface.GetID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, container2.GetID());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, container2_specular.GetID());
}
void Application::ChangeColor(float _newcolor[4])
{
	for (int i = 0; i < 4; i++)
		m_ClearColor[i] = _newcolor[i];
	ApplyChangeColor();
}

void Application::TransTest()
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
			cubePositions[j * 6 + i] = Vectorf3(2.f * i -+ 6, -2.f * j + 4, 0.0f);
	for (unsigned int i = 0; i < 24; i++)
	{
		float angle = 20.0f * i * M_PI_2 / 180;
		Matrix3x3 rotation = matrix::Rotate3dAllAxis(Vectorf3{ angle, .3f * angle, .5f * angle });
		Matrix4x4 modeltest = matrix::MatrixTRS(cubePositions[i].X(), cubePositions[i].Y(), cubePositions[i].Z(), angle, .3f * angle, .5f * angle, 1.f, 1.f, 1.f);

		Matrix4x4 MVP = camera.viewProjection * modeltest;

		shadbasic.SetMat4("MVP", MVP);
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
void Application::LightTest()
{
	sLight.point.position = camera.eye;
	sLight.direction = camera.zCamera;
	pLight.position = matrix::Rotate3D(ImGui::GetIO().DeltaTime, matrix::Axis::Y) * pLight.position;
	shadlightCube.Use();
	Matrix4x4 model = matrix::MatrixTRS(pLight.position, {}, { 1.f,1.f,1.f });
	model *= 0.2f;
	Matrix4x4 MVP = camera.viewProjection * model;
	unsigned int MVPLoc = glGetUniformLocation(shadlightCube.GetShaderProgram(), "MVP");
	shadlightCube.SetMat4("MVP", MVP);
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shadlight.Use();
	shadlight.SetVec3("objectColor", 1.0f,1.0f,1.0f/* 0.5f, 0.31f*/);
	pLight.InitShader(shadlight);
	dLight.InitShader(shadlight);
	sLight.InitShader(shadlight);
	shadlight.SetVec3("viewPos", camera.eye);
}

//VertexBufferOutput
void Application::VBOCubetest()
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

void Application::ShowImGuiControls()
{
	if (ImGui::Begin("Config"))
	{
		ImGui::Text("'S' + 'I' Keys to hide/show controls");
		if (ImGui::CollapsingHeader("Framebuffer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("FPS : %f", 1.f / ImGui::GetIO().DeltaTime);
			if (ImGui::ColorEdit4("clearColor", m_ClearColor))
				ApplyChangeColor();

		}
		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			camera.ShowImGuiControls();
		}
	}
	ImGui::End();
}

void Application::ProcessInput(GLFWwindow* _window)
{
	static double s_LastPressed = glfwGetTime();
	double timeToApply = .5;
	//App
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, true);
	if ((glfwGetKey(_window, GLFW_KEY_S) & glfwGetKey(_window, GLFW_KEY_I)) == GLFW_PRESS && (glfwGetTime() - s_LastPressed) > timeToApply)
	{
		s_LastPressed = glfwGetTime();
		m_ShowControls = !m_ShowControls;
	}
	//LearnOpenGL
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

	//Camera, should clean
	{
		double newMouseX, newMouseY;
		glfwGetCursorPos(window, &newMouseX, &newMouseY);
		mouseDeltaX = (float)(newMouseX - mouseX);
		mouseDeltaY = (float)(newMouseY - mouseY);
		mouseX = newMouseX;
		mouseY = newMouseY;
	}

	// Update camera
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		inputs.deltaX = mouseDeltaX;
		inputs.deltaY = mouseDeltaY;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		inputs.deltaX = 0.f;
		inputs.deltaY = 0.f;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	inputs.moveForward = glfwGetKey(window, GLFW_KEY_W);
	inputs.moveBackward = glfwGetKey(window, GLFW_KEY_S);
	inputs.moveRight = glfwGetKey(window, GLFW_KEY_D);
	inputs.moveLeft = glfwGetKey(window, GLFW_KEY_A);
	glfwSetScrollCallback(window, Scroll_callback);
	if (s_MouseScrollOffset)
	{
		camera.Zoom(s_MouseScrollOffset);
		s_MouseScrollOffset = 0.f;
	}
	//Cam end
}
void Application::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	s_MouseScrollOffset = (float)yoffset;
};
void Application::framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);
}
void Application::SetupImGui(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable viewports

	io.Fonts->AddFontDefault();

	// GL 3.0 + GLSL 130
	const char* const glslVersion = "#version 130";

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);
}

void Application::StartImGuiFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void Application::Render(GLFWwindow* window)
{

	ImGui::Render();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	GLFWwindow* ctxBackup = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(ctxBackup);

}
