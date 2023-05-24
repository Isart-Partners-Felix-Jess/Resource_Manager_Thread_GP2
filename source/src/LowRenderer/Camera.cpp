#include <Camera.hpp>


#include <ImGui/imgui.h>

Camera::Camera(unsigned int _width, unsigned int _height)
{
	zNear = 0.1f;
	zFar = 100.f;
	fovY = static_cast<float>(M_PI_2);
	eye = Vectorf3{ 0.f,0.f,3.f };
	center = Vectorf3{ 0.f,0.f,0.f };
	up = Vectorf3{ 0.f,1.f,0.f };
	//zCamera normalized inside SetView()

	// width and height are useful to compute projection matrix with the right aspect ratio
	width = _width;
	height = _height;
	aspect = (float)_width / _height;
	SetView();
	SetProjection();
	ComputeViewProjection();
}

void Camera::Update(float deltaTime, const CameraInputs& inputs)
{
	if (viewChanged)
		SetView();
	if (projChanged)
		SetProjection();
	if (viewChanged || projChanged)
		ComputeViewProjection();

	viewChanged = false;
	projChanged = false;
	if (inputs.NoInputs())
		return;
	viewChanged = true;
	if (inputs.deltaX)
	{
		Turn(inputs.deltaX * deltaTime * camRotationSpeed, matrix::Axis::Y);
	}
	if (inputs.deltaY)
	{
		Turn(inputs.deltaY * deltaTime * camRotationSpeed, matrix::Axis::X);
	}
	if (inputs.moveForward)
		Move(zCamera * deltaTime * camSpeed);
	if (inputs.moveBackward)
		Move(-zCamera * deltaTime * camSpeed);
	if (inputs.moveLeft)
	{
		Vectorf3 right = up.Cross_product(zCamera).Normalize();
		Move(right * deltaTime * camSpeed);
	}
	if (inputs.moveRight)
	{
		Vectorf3 left = zCamera.Cross_product(up).Normalize();
		Move(left * deltaTime * camSpeed);
	}
}
void Camera::Move(const Vectorf3& _velocity)
{
	eye = eye + _velocity;
	center = center + _velocity;
}
void Camera::Turn(float _angle, matrix::Axis _axis)
{
	//FPS View
	if (perspective)
	{
		zCamera = matrix::Rotate3D(_angle, _axis) * zCamera;
		center = eye + zCamera;
		return;
	}
	else
		//Model View
		eye = matrix::Rotate3D(_angle, _axis) * eye;
}
void Camera::Zoom(float yoffset)
{
	if (perspective)
	{
		float degToRad = M_PI / 180.f;
		fovY -= yoffset * degToRad;
		if (fovY < (/*1.0f * */degToRad))
			fovY = /*1.0f * */degToRad;
		if (fovY > 179.0f * degToRad)
			fovY = 179.0f * degToRad;
	}
	else
	{
		float zoomSpeed = 0.1f;
		orthoScale -= zoomSpeed * yoffset;
	}
	projChanged = true;
}
//Remember to compute ViewProjection after this (or after projection)
void Camera::SetView()
{
	zCamera = (center - eye).Normalize();

	Vectorf3 zC = -zCamera;
	Vectorf3 xC = (up.Cross_product(zC)).Normalize();
	Vectorf3 yC = zC.Cross_product(xC);

	Matrix4x4 result{
	{ xC.X(), xC.Y(), xC.Z(), -(xC.Dot(eye))},
	{ yC.X(), yC.Y(), yC.Z(), -(yC.Dot(eye))},
	{ zC.X(), zC.Y(), zC.Z(), -(zC.Dot(eye)) },
	{ 0.f, 0.f, 0.f, 1.f }
	};

	view = result;
}
//Remember to compute ViewProjection after this (or after view)
void Camera::SetProjection()
{
	if (perspective)
	projection = Perspective(fovY, aspect, zNear, zFar);
	else
	projection = Orthographic(-orthoScale, orthoScale, -orthoScale, orthoScale);
}
void Camera::ComputeViewProjection()
{
	viewProjection = projection * view;
}

void Camera::LookAt(float _x, float _y, float _z)
{
	LookAt({ _x,_y,_z });
}

void Camera::LookAt(const Vectorf3& _target)
{
	center = _target;
	zCamera = (center - eye).Normalize();
	viewChanged = true;
}


void Camera::ShowImGuiControls()
{
	//View
	if (ImGui::CollapsingHeader("Controls", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("WASD Keys to move along XZ Axis");
		ImGui::Text("Right click to capture mouse and");
		
	}
	if (ImGui::CollapsingHeader("View", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderFloat("Camera Speed", &camSpeed, 0.f, 10.f);
		ImGui::SliderFloat("Angle Sensibility", &camRotationSpeed, 0.01f, 1.0f);
		ImGui::SliderFloat3("eye", eye.elements, -1000.f, 1000.f);
		if (ImGui::IsItemEdited())
			viewChanged = true;
		ImGui::SliderFloat3("target", center.elements, -1000.f, 1000.f);
		if (ImGui::IsItemEdited())
			viewChanged = true;
		ImGui::SliderFloat3("up", up.elements, 0.f, 1.f);
		if (ImGui::IsItemEdited())
			viewChanged = true;

		static Vectorf3 lookAt;
		ImGui::InputFloat3("Look At :", &lookAt[0]);
		if (ImGui::IsItemEdited())
			LookAt(lookAt);
	}
	//Projection
	if (ImGui::CollapsingHeader("Projection", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::ListBoxHeader("Options"))
		{
			if (ImGui::Selectable("Perspective"))
			{
				perspective = true;
				projChanged = true;
			}
			if (ImGui::Selectable("Orthographic"))
			{
				perspective = false;
				projChanged = true;
			}
			ImGui::ListBoxFooter();
		}
		ImGui::SliderAngle("FOV", &fovY, 1.f, 179.f);
		if (ImGui::IsItemEdited() && perspective)
			projChanged = true;
		ImGui::SliderFloat("Near", &zNear, 0.f, 10.f);
		if (ImGui::IsItemEdited() && perspective)
			projChanged = true;
		ImGui::SliderFloat("Far", &zFar, 1.f, 10000.f);
		if (ImGui::IsItemEdited() && perspective)
			projChanged = true;
	}
}

Matrix4x4 Camera::Frustum(float left, float right, float bottom, float top, float near, float far)
{
	float iHorDist = 1.f / (right - left);
	float iVerDist = 1.f / (top - bottom);
	float iDepDist = 1.f / (near - far);
	float dNear = 2.f * near;
	return Matrix4x4{
		{dNear * iHorDist,0.f,(right + left) * iHorDist,0.f},
		{0.f,dNear * iVerDist,(top + bottom) * iVerDist,0.f},
		{0.f,0.f, (far + near) * iDepDist, dNear * far * iDepDist},
		{0.f,0.f, -1.f, 0.f} };
}
Matrix4x4 Camera::Perspective(float fovY, float aspect, float near, float far)
{
	float angle = M_PI_2 - fovY * 0.5f;
	//Safe tan
	float f = 0.f;
	//Very specific, I am afraid
	if (angle != M_PI_2 && angle != -M_PI_2)
		f = tan(angle);
	float iDist = 1.f / (near - far);
	return Matrix4x4{
		{f / aspect,0.f,0.f					,0.f},
		{		0.f,f  ,0.f					,0.f },
		{		0.f,0.f,(far + near) * iDist,2.f * far * near * iDist},
		{		0.f,0.f,-1.f				,0.f }
	};
}
Matrix4x4 Camera::Orthographic(float left, float right, float bottom, float top)
{
	float iHdist = 1.f / (right - left);
	float iVdist = 1.f / (top - bottom);
	float iZdist = 1.f / (zFar - zNear);

	return Matrix4x4{
		{  2 * iHdist	,0.f		,0.f		,-(right + left) * iHdist},
		{		0.f		,2 * iVdist	,0.f		,-(top + bottom) * iVdist},
		{		0.f		,0.f		,-2 * iZdist,-(zFar + zNear) * iZdist},
		{		0.f		,0.f		,0.f  		,1.f					 }
	};
}

bool CameraInputs::NoInputs() const
{
	if (deltaX != 0.f || deltaY != 0.f || moveForward || moveBackward || moveLeft || moveRight)
		return false;
}
