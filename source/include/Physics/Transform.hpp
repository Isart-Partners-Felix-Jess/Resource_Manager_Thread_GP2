#pragma once
#ifndef TRANSFORM_H
#define	TRANSFORM_H
#include<matrix.hpp>

class Transform
{
	Matrix4x4 local = Matrix4x4(true);
	Matrix4x4 global = Matrix4x4(true); //ModelMatrix

public:
	Vectorf3 translation;
	Vectorf3 rotation;
	Vectorf3 scale{ 1.f,1.f,1.f };

	Transform();
	Transform(Vectorf3 _translation, Vectorf3 _rotation = {0.f,0.f,0.f}, Vectorf3 _scale = {1.f,1.f,1.f});

	void ComputeLocal();
	void ComputeGlobal();
	
	Vectorf3 GetGlobalTranslation();
	Vectorf3 GetGlobalRotation();
	Vectorf3 GetGlobalScaling();

	Matrix4x4 ModelMatrix();

	void ComputeAll(Matrix4x4 _globalTransform);
	void SetNewLocalFrom(Matrix4x4 _globalTransform);
};
#endif //!TRANSFORM_H

