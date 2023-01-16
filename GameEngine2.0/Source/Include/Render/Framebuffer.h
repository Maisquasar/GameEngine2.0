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

		Resources::Texture* Tex = nullptr;
		Resources::Shader* shader = nullptr;

		GLuint FBO = 0;

	private:
		GLuint _RBO = 0;
	};
}