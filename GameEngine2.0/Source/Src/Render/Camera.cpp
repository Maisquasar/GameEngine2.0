#include "Include/Render/Camera.h"
#include "Include/Utils/Input.h"
#include "Include/App.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/EditorUi/SceneWindow.h"

void GetRotateAround(Math::Vec3 point, Math::Vec3 axis, float angle, Math::Vec3& pos, Math::Quat& rot)
{
	Math::Quat q = Math::Quat::AngleAxis(angle, axis);
	Math::Vec3 dif = pos - point;
	dif = q * dif;
	pos = point + dif;
	auto inverse = rot.GetInverse();
	auto tmp = inverse * q;
	rot = rot * rot.GetInverse() * q * rot;
}

Render::Camera::Camera()
{
}

Render::Camera::~Camera() {}

void Render::Camera::Initialize()
{
	this->GetTransform()->SetWorldPosition(Math::Vec3(0, 0, 10));
	FrameBuffer.Initialize(Math::Vec2(1920, 1080));
}

void Render::Camera::PreUpdate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer.FBO);
}

void Render::Camera::Update(bool firstUpdate)
{
	//Transform.Update();
	if (!firstUpdate)
	{
		auto mousePos = ImGui::GetMousePos();
		// Set Mouse Pos Modulo Monitor Size
		if (mousePos.x < 1)
			SetCursorPos(Application.GetMonitorVideoMode()->width - 2, (int)mousePos.y);
		if (mousePos.x >= Application.GetMonitorVideoMode()->width - 1)
			SetCursorPos(2, (int)mousePos.y);
		if (mousePos.y < 1)
			SetCursorPos((int)mousePos.x, Application.GetMonitorVideoMode()->height - 2);
		if (mousePos.y >= Application.GetMonitorVideoMode()->height - 1)
			SetCursorPos((int)mousePos.x, 2);
	}

	// Camera Update.
	// Set Distance
	Distance -= Utils::Input::MouseScroll * 0.5f;
	float min = 0.001f;
	if (Distance < min) Distance = min;
	Transform.RotateAround(FocusPosition, Transform.GetUpVector(), -Utils::Input::MouseDelta.x / 2);
	Transform.RotateAround(FocusPosition, Transform.GetRightVector(), Utils::Input::MouseDelta.y / 2);

	float movementSpeed = Utils::Input::IsKeyDown(ImGuiKey_LeftShift) ? 15.f : 5.f;
	if (Utils::Input::IsKeyDown(ImGuiKey_D))
	{
		FocusPosition = (FocusPosition + (-Transform.GetRightVector() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_A))
	{
		FocusPosition = (FocusPosition + (Transform.GetRightVector() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_S))
	{
		FocusPosition = (FocusPosition + (-Transform.GetForwardVector() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_W))
	{
		FocusPosition = (FocusPosition + (Transform.GetForwardVector() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_F))
	{
		FocusPosition = (FocusPosition + (-Transform.GetUpVector() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_Space))
	{
		FocusPosition = (FocusPosition + (Transform.GetUpVector() * movementSpeed * ImGui::GetIO().DeltaTime));
	}

	Transform.SetLocalPosition(FocusPosition - Transform.GetForwardVector() * Distance);
	Transform.SetLocalRotation(Math::Quat::LookRotation(FocusPosition - Transform.GetLocalPosition(), Math::Vec3::Up()));
}

Math::Mat4 Render::Camera::GetViewMatrix()
{
	Math::Mat4 temp;
	Math::Vec3 z = -GetTransform()->GetForwardVector();
	Math::Vec3 x = -GetTransform()->GetRightVector();
	Math::Vec3 y = GetTransform()->GetUpVector();
	Math::Vec3 delta = Math::Vec3(-x.DotProduct(this->GetTransform()->GetWorldPosition()), -y.DotProduct(this->GetTransform()->GetWorldPosition()), -z.DotProduct(this->GetTransform()->GetWorldPosition()));
	for (int i = 0; i < 3; i++)
	{
		temp.at(i, 0) = x[i];
		temp.at(i, 1) = y[i];
		temp.at(i, 2) = z[i];
		temp.at(3, i) = delta[i];
	}
	temp.at(3, 3) = 1;
	return temp;
}

Math::Mat4 Render::Camera::GetProjection()
{
	float s = 1.0f / ((AspectRatio)*atanf(Math::ToRadians(FOV / 2.0f)));
	float s2 = 1.0f / atanf(Math::ToRadians(FOV / 2.0f));
	float param1 = -(Far + Near) / (Far - Near);
	float param2 = -(2 * Near * Far) / (Far - Near);
	Math::Mat4 out;
	out.at(0, 0) = s;
	out.at(1, 1) = s2;
	out.at(2, 2) = param1;
	out.at(2, 3) = -1;
	out.at(3, 2) = param2;
	return out;
}

Math::Mat4 Render::Camera::GetModelMatrix()
{
	return GetTransform()->GetModelMatrix();
}

Math::Vec3 Render::Camera::GetUp()
{
	return GetModelMatrix() * Math::Vec3::Up();
}

Math::Vec3 Render::Camera::GetRight()
{
	return GetModelMatrix() * Math::Vec3::Right();
}

Math::Vec3 Render::Camera::UnProject(Math::Vec2 point)
{    
	// Convert the screen position to normalized device coordinates
	Math::Vec3 ndc;
	ndc.x = (point.x / EditorUi::Editor::GetSceneWindow()->GetSize().x) * 2 - 1;
	ndc.y = ((EditorUi::Editor::GetSceneWindow()->GetSize().y - point.y) / EditorUi::Editor::GetSceneWindow()->GetSize().y) * 2 - 1;
	ndc.z = 10;

	// Use the inverse of the projection matrix to convert from NDC to world space
	auto invProjectionMatrix = GetProjection().CreateInverseMatrix();
	Math::Vec4 worldPos = invProjectionMatrix * ndc;

	// Divide by the w-coordinate to get the final world position
	worldPos.Homogenize();

	// Use the inverse of the view matrix to transform the world position into the correct space
	auto invViewMatrix = GetViewMatrix().CreateInverseMatrix();
	worldPos = invViewMatrix * worldPos;

	return Math::Vec3(worldPos);
}

bool Render::Camera::IsVisible()
{
	 if (EditorUi::Editor::GetSceneWindow()->Window) return EditorUi::Editor::GetSceneWindow()->Window->DockTabIsVisible;
	 return true;
}
