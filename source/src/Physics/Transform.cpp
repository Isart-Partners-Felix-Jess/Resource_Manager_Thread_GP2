#include <Transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>


void Transform::ComputeLocal()
{
	local = matrix::MatrixTRS(translate, rotate * (M_PI/180), scale);
}

void Transform::ComputeGlobal()
{
	global = matrix::MatrixTRS(translate, rotate * (M_PI / 180), scale);
}

Vectorf3 Transform::GetLocalTranslation()
{
	return Vectorf3(local[4][0], local[4][1], local[4][2]);
}

Vectorf3 Transform::GetLocalRotation()
{
	float xAngle = xAxis.GetAngleVector({ 1.f,0.f,0.f });
	float yAngle = yAxis.GetAngleVector({ 0.f,1.f,0.f });
	float zAngle = zAxis.GetAngleVector({ 0.f,0.f,1.f });

	return Vectorf3(xAngle,yAngle,zAngle)*(180*M_1_PI);
}

Vectorf3 Transform::GetLocalScaling()
{
	return Vectorf3(local.Column(0).Magnitude(), local.Column(1).Magnitude() , local.Column(2).Magnitude());
}
Vectorf3 Transform::GetGlobalTranslation()
{
	return Vectorf3(global[4][0], global[4][1], global[4][2]);
}

Vectorf3 Transform::GetGlobalRotation()
{
	float xAngle = xAxis.GetAngleVector({ 1.f,0.f,0.f });
	float yAngle = yAxis.GetAngleVector({ 0.f,1.f,0.f });
	float zAngle = zAxis.GetAngleVector({ 0.f,0.f,1.f });

	return Vectorf3(xAngle, yAngle, zAngle)*(180 * M_1_PI);
}

Vectorf3 Transform::GetGlobalScaling()
{
	return Vectorf3(global.Column(0).Magnitude(), global.Column(1).Magnitude(), global.Column(2).Magnitude());
}

Matrix4x4 Transform::ModelMatrix()
{
	return global;
}

void Transform::ComputeAll(Matrix4x4 _transform)
{
	local = _transform * local;
	global = _transform * global;
	xAxis = _transform * xAxis;
	yAxis = _transform * yAxis;
	zAxis = _transform * zAxis;
}
