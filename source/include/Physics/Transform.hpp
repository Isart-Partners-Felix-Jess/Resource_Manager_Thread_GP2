#pragma once
#ifndef TRANSFORM_H
#define	TRANSFORM_H
#include<matrix.hpp>

class Transform
{
	Matrix4x4 local = Matrix4x4(true);
	Matrix4x4 global = Matrix4x4(true); //ModelMatrix

	Vectorf3 translate;
	Vectorf3 rotate;
	Vectorf3 scale{ 1.f,1.f,1.f };

	Vectorf3 xAxis{ 1.f,0.f,0.f };
	Vectorf3 yAxis{ 0.f,1.f,0.f };
	Vectorf3 zAxis{ 0.f,0.f,1.f };
public:

	void ComputeLocal();
	void ComputeGlobal();
	Vectorf3 GetLocalTranslation();
	Vectorf3 GetLocalRotation();
	Vectorf3 GetLocalScaling();

	Vectorf3 GetGlobalTranslation();
	Vectorf3 GetGlobalRotation();
	Vectorf3 GetGlobalScaling();

	Matrix4x4 ModelMatrix();

	void ComputeAll(Matrix4x4 _globalTransform);
};
#endif //!TRANSFORM_H

