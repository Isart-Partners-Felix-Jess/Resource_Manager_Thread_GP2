#include <Transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

Transform::Transform(Vectorf3 _translation, Vectorf3 _rotationInDeg, Vectorf3 _scale)
{
	translation = _translation;
	rotation = _rotationInDeg;
	scale = _scale;
	ComputeLocal();
	m_global = m_local;
}

void Transform::ComputeLocal() {
	m_local = matrix::MatrixTRS(translation, rotation * (M_PI / 180), scale);
}

void Transform::ComputeGlobal()
{
	m_global = matrix::MatrixTRS(translation, rotation * (M_PI / 180), scale);
	m_normalMatrix = m_global.Inversion().Transposed();
}

Vectorf3 Transform::GetGlobalTranslation() {
	return Vectorf3(m_global[4][0], m_global[4][1], m_global[4][2]);
}

Vectorf3 Transform::GetGlobalRotation()
{
	Vectorf3 xAxis = Vectorf3(m_global.Column(0)[0], m_global.Column(0)[1], m_global.Column(0)[2]);
	Vectorf3 yAxis = Vectorf3(m_global.Column(1)[0], m_global.Column(1)[1], m_global.Column(1)[2]);
	Vectorf3 zAxis = Vectorf3(m_global.Column(2)[0], m_global.Column(2)[1], m_global.Column(2)[2]);

	float xAngle = xAxis.GetAngleVector({ 1.f,0.f,0.f });
	float yAngle = yAxis.GetAngleVector({ 0.f,1.f,0.f });
	float zAngle = zAxis.GetAngleVector({ 0.f,0.f,1.f });

	return Vectorf3(xAngle, yAngle, zAngle) * (180 * M_1_PI);
}

Vectorf3 Transform::GetGlobalScaling() {
	return Vectorf3(m_global.Column(0).Magnitude(), m_global.Column(1).Magnitude(), m_global.Column(2).Magnitude());
}

Matrix4x4 Transform::ModelMatrix() {
	return m_global;
}

Matrix4x4 Transform::NormalMatrix() {
	return m_normalMatrix;
}

void Transform::ComputeAll(Matrix4x4 _globalTransform) {
	m_global = _globalTransform * m_local;
}

void Transform::SetNewLocalFrom(Matrix4x4 _globalTransform) {
	m_local = m_global * _globalTransform.Inversion();
}