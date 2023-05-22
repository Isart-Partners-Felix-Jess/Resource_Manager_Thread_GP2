#include <Application.hpp>
#include <iostream>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <Shader.hpp>

//TODO: DELETE THIS !!!!!!
unsigned int EBO;
unsigned int VAO;
unsigned int VBO;
Shader shadbasic;
Shader shadyellow;

Application::Application() :Application(800, 600)
{
}

Application::Application(int _width, int _height)
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
	Shadertest();
	glViewport(0, 0, _width, _height);
	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	SetupImGui(window);
}

Application::~Application()
{
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
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		StartImGuiFrame();
		processInput(window);
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
		// positions         // colors
		 0.5f,  0.5f, 0.0f,	 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};
	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};
	VAO = VAOtest(VBO);
	VBOtest(VBO, shadbasic);
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
int Application::VBOtest(unsigned int& _VBOid, Shader& _shader)
{
	float vertices[] = {
		// positions         // colors
		 0.5f,  0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f    // top 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenBuffers(1, &_VBOid);
	glBindBuffer(GL_ARRAY_BUFFER, _VBOid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//_shader.LoadResource("assets/shaders/basic");
		// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	return _VBOid;
}
//VertexArrayOutput
int Application::VAOtest(unsigned int& _VBOid)
{
	float vertices[] = {
		// positions         // colors
		 0.5f,  0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f    // top 
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	EBOtest();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	return VAO;
}
void Application::ChangeColor(float _newcolor[4])
{
	for (int i = 0; i < 4; i++)
		m_ClearColor[i] = _newcolor[i];
	ApplyChangeColor();
}

void Application::ShowImGuiControls()
{
	if (ImGui::Begin("Config"))
	{
		ImGui::Text("'S' + 'C' Keys to hide/show controls");
		if (ImGui::CollapsingHeader("Framebuffer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::ColorEdit4("clearColor", m_ClearColor))
				ApplyChangeColor();

		}
		ImGui::End();
	}
}

void Application::processInput(GLFWwindow* _window)
{
	static double s_LastPressed = glfwGetTime();
	double timeToApply = .5;

	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, true);
	if ((glfwGetKey(_window, GLFW_KEY_S) & glfwGetKey(_window, GLFW_KEY_C)) == GLFW_PRESS && (glfwGetTime() - s_LastPressed) > timeToApply)
	{
		s_LastPressed = glfwGetTime();
		m_ShowControls = !m_ShowControls;
	}
}
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
//same with mouse position
void Application::NewFrame(bool mouseCaptured)
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	if (mouseCaptured)
		ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	ImGui::NewFrame();
}
void Application::Render(GLFWwindow* window)
{

	ImGui::Render();
	//UpdateGlFrameBuffer(window);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//OpenGL test part
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	//int vertexColorLocation = glGetUniformLocation(shadbasic.GetShaderProgram(), "ourColor");
	shadbasic.Use();
	//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	//shadyellow.Use();
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	//

	GLFWwindow* ctxBackup = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(ctxBackup);

}
