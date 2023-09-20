#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <Scene.hpp>
#include <ResourcesManager.hpp>
#include <Assertion.hpp>

class Application
{
private:
	GLFWwindow* m_window;
	int m_Width;
	int m_Height;
	float m_ClearColor[4] = { 0.05f, 0.f, 0.2f, 1.f };
	float m_deltaTime = 0.f;
	Scene m_scene;

	CameraInputs m_inputs;
	double m_mouseX = 0.0;
	double m_mouseY = 0.0;
	float m_mouseDeltaX = 0.f;
	float m_mouseDeltaY = 0.f;
	static float s_m_MouseScrollOffset;

	bool m_ShowControls = true;
	void ProcessInput(GLFWwindow* _window);
	static void Scroll_callback(GLFWwindow* _window, double _xoffset, double _yoffset);

	static void framebuffer_size_callback(GLFWwindow* _window, int _width, int _height);
	void SetupImGui(GLFWwindow* _window);
	static void StartImGuiFrame();
	void Render(GLFWwindow* _window);
	void ApplyChangeColor();

public:
	Application() : Application(800, 600) {};
	Application(int _width, int _height);
	~Application();

	void Destroy();
	void Update();

	// Not a very useful function TBH, just use glClearColor
	void ChangeColor(float _newcolor[4]);

	void ShowImGuiControls();
};