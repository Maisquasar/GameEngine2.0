#include "Include/Render/CameraEditor.h"
#include "Include/App.h"

Render::CameraEditor::CameraEditor()
{
	this->Transform.SetWorldPosition(Math::Vector3(0, 0, -10));
}

Render::CameraEditor::~CameraEditor(){}

void Render::CameraEditor::Update()
{
	Transform.Update();
	// Set Distance
	Distance -= Utils::Input::MouseScroll * 0.5f;
	float min = 0.001f;
	if (Distance < min) Distance = min;
	auto UpTransform = Transform.GetWorldRotation() * Math::Vector3(0, 1, 0);
	auto RightTransform = Transform.GetWorldRotation() * Math::Vector3(1, 0, 0);
	Transform.RotateAround(FocusPosition, UpTransform, -Utils::Input::MouseDelta.x / 2);
	Transform.RotateAround(FocusPosition, RightTransform, Utils::Input::MouseDelta.y / 2);

	Math::Vector3 cameraDir = Transform.GetWorldPosition() - FocusPosition;

	float dSpeed = ImGui::GetIO().DeltaTime * Speed * (Utils::Input::IsKeyDown(ImGuiKey_RightShift) ? 5.0f : 1.0f);
	Math::Vector3 delta = Math::Vector3(dSpeed * Utils::Input::IsKeyDown(ImGuiKey_D) - dSpeed * Utils::Input::IsKeyDown(ImGuiKey_A), dSpeed * Utils::Input::IsKeyDown(ImGuiKey_Space) - dSpeed * Utils::Input::IsKeyDown(ImGuiKey_F), dSpeed * Utils::Input::IsKeyDown(ImGuiKey_S) - dSpeed * Utils::Input::IsKeyDown(ImGuiKey_W)) / 20;
	auto rot = Transform.GetWorldRotation();
	FocusPosition = (FocusPosition) + (rot * Math::Vector3(0, 0, -1) * delta.z) + (rot * Math::Vector3(-1, 0, 0) * delta.x) + (rot * Math::Vector3(0, 1, 0) * delta.y);

	Transform.SetWorldPosition((FocusPosition + cameraDir.Normalize() * Distance));
}

Math::Matrix4 Render::CameraEditor::GetViewMatrix()
{
	Math::Matrix4 temp;
	Math::Vector3 z = (this->Transform.GetWorldPosition() - FocusPosition).UnitVector();
	Math::Vector3 x = UpVector.CrossProduct(z).UnitVector();
	Math::Vector3 y = z.CrossProduct(x);
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