#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#define _USE_MATH_DEFINES
#include <matrix.hpp>

struct CameraInputs
{
    float deltaX;
    float deltaY;
    bool moveForward;
    bool moveBackward;
    // ... and more if needed
};

struct Camera
{
    unsigned int width, height;
    float aspect;
    float fovY;
    float zNear, zFar;
    Vectorf3 eye, center, up;
    Vectorf3 zCamera;

    float camSpeed = 0.5f;
    float camRotationSpeed = 0.1f;

    Matrix4x4 view;
    Matrix4x4 projection;
    bool perspective = true;
    Matrix4x4 viewProjection;

    Camera(unsigned int width, unsigned int height);

    void Update(float deltaTime, const CameraInputs& inputs);
    void SetView();
    void SetProjection();
    void ComputeViewProjection();

    void ShowImGuiControls();

private:
    Matrix4x4 Frustum(float left, float right, float bottom, float top, float near, float far);
    Matrix4x4 Perspective(float fovY, float aspect, float near, float far);
    Matrix4x4 Orthographic(float left, float right, float bottom, float top);
};
#endif // !CAMERA_H