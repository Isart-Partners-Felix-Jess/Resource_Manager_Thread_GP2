#pragma once

#ifndef APP_H
#define APP_H
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <Assertion.hpp>
#include <Camera.hpp>

class Shader;

class Application
{
private:
	GLFWwindow* window;
	int m_Width;
	int m_Height;
	float m_ClearColor[4] = { 0.2f, 0.3f, 0.3f, 1.0f
	};
	//Cam part
	Camera camera;
	CameraInputs inputs;
	double mouseX = 0.0;
	double mouseY = 0.0;
	float mouseDeltaX = 0.0;
	float mouseDeltaY = 0.0;
	static float s_MouseScrollOffset;

	bool m_ShowControls = true;
	void ProcessInput(GLFWwindow* window);
	static void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void SetupImGui(GLFWwindow* window);
	static void StartImGuiFrame();
	static void NewFrame(bool mouseCaptured);
	void Render(GLFWwindow* window);
	void ApplyChangeColor();

	//LearnOpenGl TODO: replace
	void Shadertest();
	void VBOtest();
	//void VBOCubetest();
	void VAOtest();
	void lightVAOtest();
	void EBOtest();
	void Texturetest();
	void TransTest();
	void VBOCubetest();
	void LightTest();

public:
	Application();
	Application(int _width, int _height);
	~Application();
	
	void Destroy();
	void Update();
	//Not so useful function TBH, just use glClearColor
	void ChangeColor(float _newcolor[4]);
	void ShowImGuiControls();
};

#endif //App_h