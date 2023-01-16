#pragma once
#include <Windows.h>
#include <glad/glad.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "../Math/Math.h"
#include "../Resources/ResourceManager.h"

namespace Render {
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void Initialize(Math::IVec2);
		void Draw();

		Math::Vec2 GetSize();
		Math::Vec2 GetPos();

		Math::Vec2 GetMousePosition();

		ImGuiWindow* Window = nullptr;

		Resources::Texture* Tex = nullptr;

		Resources::Shader* shader = nullptr;
		GLuint FBO = 0;

		bool UpdateCameraEditor = false;
		bool IsHovered = false;
		bool IsFocused = false;

	private:
		GLuint _RBO = 0;
		Math::Vec2 _size;
		Math::Vec2 _pos;
	};
}