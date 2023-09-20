#pragma once

#include <matrix.hpp>

class Transform
{
private:
	Matrix4x4 m_local = Matrix4x4(true);
	Matrix4x4 m_global = Matrix4x4(true); // ModelMatrix
	Matrix4x4 m_normalMatrix = Matrix4x4(true);

public:
	// Reminder to computeLocal if you change those
	Vectorf3 translation;
	Vectorf3 rotation;
	Vectorf3 scale{ 1.f,1.f,1.f };

	Transform() {};
	Transform(Vectorf3 _translation, Vectorf3 _rotation = { 0.f,0.f,0.f }, Vectorf3 _scale = { 1.f,1.f,1.f });

	void ComputeLocal();
	void ComputeGlobal();

	Vectorf3 GetGlobalTranslation();
	Vectorf3 GetGlobalRotation();
	Vectorf3 GetGlobalScaling();

	Matrix4x4 ModelMatrix();
	Matrix4x4 NormalMatrix();

	void ComputeAll(Matrix4x4 _globalTransform);
	void SetNewLocalFrom(Matrix4x4 _globalTransform);
};