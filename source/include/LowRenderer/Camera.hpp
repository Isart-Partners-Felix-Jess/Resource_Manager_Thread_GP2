#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#define _USE_MATH_DEFINES
#include <matrix.hpp>

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

    float camSpeed = 0.5f;
    float camRotationSpeed = 0.1f;
    float orthoScale = 2.f;

    Matrix4x4 view;
    bool viewChanged;
    Matrix4x4 projection;
    bool perspective = true;
    bool projChanged;
    Matrix4x4 viewProjection;

    Camera(unsigned int width, unsigned int height);

    void Update(float deltaTime, const CameraInputs& inputs);
    void SetView();
    void SetProjection();
    void ComputeViewProjection();
    void LookAt(float _x, float _y, float _z);
    void LookAt(const Vectorf3& _target);

    void ShowImGuiControls();

    void Zoom(float yoffset);
private:
    Matrix4x4 Frustum(float left, float right, float bottom, float top, float near, float far);
    Matrix4x4 Perspective(float fovY, float aspect, float near, float far);
    Matrix4x4 Orthographic(float left, float right, float bottom, float top);
    void Move(const Vectorf3& _velocity);
    void Turn(float _angle, matrix::Axis _axis);
};
#endif // !CAMERA_H