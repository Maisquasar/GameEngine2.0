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

		void Initialize();
		void Draw();

		ImGuiWindow* Window = nullptr;

		Resources::Texture* Tex;
		ImColor ClearColor = ImColor(255, 255, 255, 255);

		Resources::Shader* shader;
		GLuint FBO;

	private:

		GLuint _VBO;
		GLuint _RBO;
		GLuint _VAO;
		GLuint _Color;
	};
}