#include "Include/Render/CameraEditor.h"
#include "Include/App.h"

void GetRotateAround(Math::Vector3 point, Math::Vector3 axis, float angle, Math::Vector3& pos, Math::Quaternion& rot)
{
	Math::Quaternion q = Math::Quaternion::AngleAxis(angle, axis);
	Math::Vector3 dif = pos - point;
	dif = q * dif;
	pos = point + dif;
	auto inverse = rot.GetInverse();
	auto tmp = inverse * q;
	rot = rot * rot.GetInverse() * q * rot;
}

Render::CameraEditor::CameraEditor()
{
	this->Transform.SetWorldPosition(Math::Vector3(0, 0, 10));
}

Render::CameraEditor::~CameraEditor() {}

void Render::CameraEditor::Update(bool firstUpdate)
{
	//Transform.Update();

	if (!firstUpdate)
	{
		auto mousePos = ImGui::GetMousePos();
		// Set Mouse Pos Modulo Monitor Size
		if (mousePos.x < 1)
			SetCursorPos(App::GetMonitorVideoMode()->width - 2, (int)mousePos.y);
		if (mousePos.x >= App::GetMonitorVideoMode()->width - 1)
			SetCursorPos(2, (int)mousePos.y);
		if (mousePos.y < 1)
			SetCursorPos((int)mousePos.x, App::GetMonitorVideoMode()->height - 2);
		if (mousePos.y >= App::GetMonitorVideoMode()->height - 1)
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
		FocusPosition = (FocusPosition + (Transform.GetRightVector().Negate() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_A))
	{
		FocusPosition = (FocusPosition + (Transform.GetRightVector() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_S))
	{
		FocusPosition = (FocusPosition + (Transform.GetForwardVector().Negate() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_W))
	{
		FocusPosition = (FocusPosition + (Transform.GetForwardVector() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_F))
	{
		FocusPosition = (FocusPosition + (Transform.GetUpVector().Negate() * movementSpeed * ImGui::GetIO().DeltaTime));
	}
	if (Utils::Input::IsKeyDown(ImGuiKey_Space))
	{
		FocusPosition = (FocusPosition + (Transform.GetUpVector() * movementSpeed * ImGui::GetIO().DeltaTime));
	}

	Transform.SetLocalPosition(FocusPosition - Transform.GetForwardVector() * Distance);
	Transform.SetLocalRotation(Math::Quaternion::LookRotation(FocusPosition - Transform.GetLocalPosition(), Math::Vector3::Up()));
}

Math::Matrix4 Render::CameraEditor::GetViewMatrix()
{
	Math::Matrix4 temp;
	Math::Vector3 z = Transform.GetForwardVector().Negate();
	Math::Vector3 x = Transform.GetRightVector().Negate();
	Math::Vector3 y = Transform.GetUpVector();
	Math::Vector3 delta = Math::Vector3(-x.DotProduct(this->Transform.GetWorldPosition()), -y.DotProduct(this->Transform.GetWorldPosition()), -z.DotProduct(this->Transform.GetWorldPosition()));
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

Math::Matrix4 Render::CameraEditor::GetProjection()
{
	float s = 1.0f / ((AspectRatio)*atanf(Math::ToRadians(FOV / 2.0f)));
	float s2 = 1.0f / atanf(Math::ToRadians(FOV / 2.0f));
	float param1 = -(Far + Near) / (Far - Near);
	float param2 = -(2 * Near * Far) / (Far - Near);
	Math::Matrix4 out;
	out.at(0, 0) = s;
	out.at(1, 1) = s2;
	out.at(2, 2) = param1;
	out.at(2, 3) = -1;
	out.at(3, 2) = param2;
	return out;
}

Math::Matrix4 Render::CameraEditor::GetModelMatrix()
{
	return Transform.GetModelMatrix();
}