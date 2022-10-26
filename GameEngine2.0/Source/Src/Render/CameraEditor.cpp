#include "Include/Render/CameraEditor.h"
#include "Include/App.h"

Render::CameraEditor::CameraEditor(){}

Render::CameraEditor::~CameraEditor(){}

void Render::CameraEditor::Update()
{
	// Update Distance.
	Distance = Math::Cut(Distance - Utils::Input::MouseScroll / 3, 0.001f, 100.0f);
	// Update Rotation.
	Rotation = Rotation + Math::Vector2(-Utils::Input::MouseDelta.x / 10, Utils::Input::MouseDelta.y / 10);
	Rotation = Math::Vector2(Math::Mod(Rotation.x, 360), Math::Cut(Rotation.y, -89.99f, 89.99f));
	// Update FocusPosition.
	float dSpeed = ImGui::GetIO().DeltaTime * Speed * (Utils::Input::IsKeyDown(ImGuiKey_RightShift) ? 5.0f : 1.0f);
	Math::Vector3 delta = Math::Vector3(dSpeed * Utils::Input::IsKeyDown(ImGuiKey_D) - dSpeed * Utils::Input::IsKeyDown(ImGuiKey_A), dSpeed * Utils::Input::IsKeyDown(ImGuiKey_Space) - dSpeed * Utils::Input::IsKeyDown(ImGuiKey_F), dSpeed * Utils::Input::IsKeyDown(ImGuiKey_S) - dSpeed * Utils::Input::IsKeyDown(ImGuiKey_W)) / 20;
	FocusPosition = FocusPosition + Math::Vector3(cosf(Math::ToRadians(Rotation.x)) * delta.x + sinf(Math::ToRadians(Rotation.x)) * delta.z, delta.y, -sinf(Math::ToRadians(Rotation.x)) * delta.x + cosf(Math::ToRadians(Rotation.x)) * delta.z);
	// Update Position.
	Position = FocusPosition + Math::Vector3(sinf(Math::ToRadians(Rotation.x)) * cosf(Math::ToRadians(Rotation.y)), sinf(Math::ToRadians(Rotation.y)), cosf(Math::ToRadians(Rotation.x)) * cosf(Math::ToRadians(Rotation.y))) * (Distance == 0.0f ? 0.001f : Distance);
}

Math::Matrix4 Render::CameraEditor::GetViewMatrix()
{
	Math::Matrix4 temp;
	Math::Vector3 z = (Position - FocusPosition).UnitVector();
	Math::Vector3 x = UpVector.CrossProduct(z).UnitVector();
	Math::Vector3 y = z.CrossProduct(x);
	Math::Vector3 delta = Math::Vector3(-x.DotProduct(Position), -y.DotProduct(Position), -z.DotProduct(Position));
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
	return Math::Matrix4::CreateTransformMatrix(Position, Math::Vector3(Rotation.y, Rotation.x - 180, 0), Math::Vector3(1));
}