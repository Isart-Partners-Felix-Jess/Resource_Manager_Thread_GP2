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
	bool mouseCaptured = false;
	double mouseX = 0.0;
	double mouseY = 0.0;
	float mouseDeltaX = 0.0;
	float mouseDeltaY = 0.0;

	bool m_ShowControls = true;
	void processInput(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void SetupImGui(GLFWwindow* window);
	static void StartImGuiFrame();
	static void NewFrame(bool mouseCaptured);
	void Render(GLFWwindow* window);
	void ApplyChangeColor();

	//LearnOpenGl TODO: replace
	void Shadertest();
	void VBOtest();
	void VAOtest();
	void EBOtest();
	void Texturetest();
	void TransTest();

public:
	Application();
	Application(int _width, int _height);
	~Application();
	
	void Update();
	//Not so useful function TBH, just use glClearColor
	void ChangeColor(float _newcolor[4]);
	void ShowImGuiControls();
};

#endif //App_h