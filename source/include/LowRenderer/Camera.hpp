#pragma once

#define _USE_MATH_DEFINES
#include <matrix.hpp>

#include <ImGui/imgui.h>

struct CameraInputs
{
	float deltaX;
	float deltaY;
	float zoom;
	bool moveForward;
	bool moveBackward;
	bool moveLeft;
	bool moveRight;
	// ... and more if needed
	bool NoInputs() const;
};

struct Camera
{
	unsigned int width, height;
	float aspect;
	float fovY;
	float zNear, zFar;
	Vectorf3 eye, center, up;
	Vectorf3 zCamera;

	float camSpeed = 8.f;
	float camRotationSpeed = 0.1f;
	float orthoScale = 2.f;

	Matrix4x4 view;
	bool viewChanged;
	Matrix4x4 projection;
	bool perspective = true;
	bool projChanged;
	Matrix4x4 viewProjection;

	Camera(unsigned int _width, unsigned int _height);

	void Update(float _deltaTime, const CameraInputs& _inputs);
	void SetView();
	void SetProjection();
	void ComputeViewProjection();
	void LookAt(float _x, float _y, float _z);
	void LookAt(const Vectorf3& _target);

	void ShowImGuiControls();

	void Zoom(float _yoffset);

private:
	Matrix4x4 Frustum(float _left, float _right, float _bottom, float _top, float _near, float _far);
	Matrix4x4 Perspective(float _fovY, float _aspect, float _near, float _far);
	Matrix4x4 Orthographic(float _left, float _right, float _bottom, float _top);

	void Move(const Vectorf3& _velocity);
	void Turn(float _angle, matrix::Axis _axis);
};