#pragma once
#include <Windows.h>
#include <glad/glad.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "../Math/Math.h"
namespace Resources
{
	class Texture;
	class Shader;
}
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
		GLuint _VAO;
		GLuint _VBO;

	private:
		GLuint _RBO = 0;
	};
}