#include "stdafx.h"
#include "Include/Utils/Input.h"
#include "Include/App.h"

float Utils::Input::MouseScroll;
Math::Vec2 Utils::Input::MouseDelta;
Math::Vec2 Utils::Input::MousePosition;


Utils::Input::Input() {}

Utils::Input::~Input() {}


void Utils::Input::Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseScroll = (float)yoffset;
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += (float)xoffset;
	io.MouseWheel += (float)yoffset;
}

void Utils::Input::Update()
{
	MouseScroll = 0;
	auto newMouse = ImGui::GetMousePos();
	MouseDelta = Math::Vec2((float)(newMouse.x - MousePosition.x), (float)(newMouse.y - MousePosition.y));
	MousePosition = Math::Vec2(newMouse.x, newMouse.y);


	if (IsKeyPressed(ImGuiKey_F4))
	{
		PrintLog("Recompiling Shaders");
		Application.GetResourceManager()->RecompileShaders();
	}
}

bool Utils::Input::IsKeyDown(int key)
{
	return ImGui::IsKeyDown((ImGuiKey)key);
}

bool Utils::Input::IsKeyPressed(int key)
{
	return ImGui::IsKeyPressed((ImGuiKey)key);
}

bool Utils::Input::IsKeyReleased(int key)
{
	return ImGui::IsKeyReleased((ImGuiKey)key);
}
