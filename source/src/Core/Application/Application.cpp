#include <Application.hpp>
#include <iostream>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <Shader.hpp>
#include <Texture.hpp>
#include <matrix.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

//TODO: DELETE THIS !!!!!!
unsigned int EBO;
unsigned int VAO;
unsigned int VBO;
Shader shadbasic;
Shader shadyellow;
float mixValue = 0.2f;
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
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
	Shadertest();
	Texturetest();
	//TransTest();

	glViewport(0, 0, _width, _height);
	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	SetupImGui(window);
}

Application::~Application()
{
	//LearnOpenGL Tuto
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shadbasic.GetShaderProgram());
	glDeleteProgram(shadyellow.GetShaderProgram());

	//IMGUI Destroy
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

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
	VAOtest();
	VBOtest();
	shadbasic.LoadResource("assets/shaders/basic");
	shadyellow.SetFragmentShader("assets/shaders/yellow.frag");
	shadyellow.SetVertexShader("assets/shaders/basic.vert");
	shadyellow.Link();
	//Render part
	shadbasic.Use();
	//shadyellow.Use();
	glBindVertexArray(VAO);

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
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	//glBindBuffer(GL_ARRAY_BUFFER, VAO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//// 3. then set our vertex attributes pointers
	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//// Texture attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	EBOtest();
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
}
void Application::Texturetest()
{
	Texture container("container.jpg");
	Texture awesomeface("awesomeface.png", true);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, container.GetID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, awesomeface.GetID());

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void Application::ChangeColor(float _newcolor[4])
{
	for (int i = 0; i < 4; i++)
		m_ClearColor[i] = _newcolor[i];
	ApplyChangeColor();
}

void Application::TransTest()
{
	//Only on movement / Imgui
	//camera.SetView();
	//Only on Imgui
	//camera.SetProjection();
	Matrix4x4 viewProjection = camera.viewProjection;

	Matrix4x4 modeltest = matrix::MatrixTRS(0.f, -0.f, 0.f, (float)glfwGetTime(), 0.f, 0.f, 1.f, 1.f, 1.f);
	unsigned int transformLoc = glGetUniformLocation(shadbasic.GetShaderProgram(), "transform");

	Matrix4x4 trans = viewProjection * modeltest;

	float elements[16];
	for (int i = 0; i < 16; i++)
	{
		elements[i] = trans[i % 4][i / 4];
	}
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, elements);
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
		ImGui::End();
	}
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
	//UpdateGlFrameBuffer(window);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//OpenGL test part
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	//int vertexColorLocation = glGetUniformLocation(shadbasic.GetShaderProgram(), "ourColor");
	shadbasic.Use();
	shadbasic.SetInt("texture1", 0);
	shadbasic.SetInt("texture2", 1);
	shadbasic.SetFloat("mixValue", mixValue);
	TransTest();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//shadyellow.Use();
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	//

	GLFWwindow* ctxBackup = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(ctxBackup);

}
