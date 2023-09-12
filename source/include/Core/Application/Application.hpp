#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <Scene.hpp>
#include <ResourcesManager.hpp>
#include <Assertion.hpp>
#include <Scene.hpp>

class Application
{
private:
	GLFWwindow* window;
	int m_Width;
	int m_Height;
	float m_ClearColor[4] = { 0.05f, 0.f, 0.2f, 1.f };
	float deltaTime = 0.f;
	Scene scene;

	//Cam part
	CameraInputs inputs;
	double mouseX = 0.0;
	double mouseY = 0.0;
	float mouseDeltaX = 0.f;
	float mouseDeltaY = 0.f;
	static float s_MouseScrollOffset;

	bool m_ShowControls = true;
	void ProcessInput(GLFWwindow* window);
	static void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void SetupImGui(GLFWwindow* window);
	static void StartImGuiFrame();
	void Render(GLFWwindow* window);
	void ApplyChangeColor();

public:
	Application() : Application(800, 600) {};
	Application(int _width, int _height);
	~Application() {
		Destroy();
	};

	void Destroy();
	void Update();
	//Not so useful function TBH, just use glClearColor
	void ChangeColor(float _newcolor[4]);
	void ShowImGuiControls();
};