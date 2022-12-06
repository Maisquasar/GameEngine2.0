#pragma once
#include <glad/glad.h>
#include <ImGui/imgui_internal.h>
#include "../Math/Math.h"
#include "../Resources/ResourceManager.h"

namespace Render {
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void Initialize(Math::Integer2);
		void Draw();

		Math::Vector2 GetSize();
		Math::Vector2 GetPos();

		Math::Vector2 GetMousePosition();

		ImGuiWindow* Window = nullptr;

		Resources::Texture* Tex = nullptr;
		ImColor ClearColor = ImColor(255, 255, 255, 255);

		Resources::Shader* shader = nullptr;
		GLuint FBO = 0;

		bool UpdateCameraEditor = false;

	private:
		GLuint _RBO = 0;
		Math::Vector2 _size;
		Math::Vector2 _pos;
	};
}