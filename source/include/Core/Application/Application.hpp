#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <Assertion.hpp>
class Shader;

class Application
{
private:
	GLFWwindow* window;
	int m_Width;
	int m_Height;
	float m_ClearColor[4] = { 0.2f, 0.3f, 0.3f, 1.0f
	};
	bool m_ShowControls = true;
	void processInput(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void SetupImGui(GLFWwindow* window);
	static void StartImGuiFrame();
	static void NewFrame(bool mouseCaptured);
	void Render(GLFWwindow* window);
	void ApplyChangeColor();

	//LearnOpenGl
	void Shadertest();
	void EBOtest();
	int VBOtest(unsigned int& _VBOid, Shader& _shader);
	int VAOtest(unsigned int& _VBOid);


public:
	Application();
	Application(int _width, int _height);
	~Application();
	
	void Update();
	//Not so useful function TBH, just use glClearColor
	void ChangeColor(float _newcolor[4]);
	void ShowImGuiControls();
};

