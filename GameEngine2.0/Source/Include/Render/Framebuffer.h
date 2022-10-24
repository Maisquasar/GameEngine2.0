#pragma once
#include <glad/glad.h>
#include "../Math/Math.h"
#include "../Resources/ResourceManager.h"

namespace Render {
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		void Initialize();
		void Draw(Math::Integer2);

		Resources::Texture* Tex;
		ImColor ClearColor;

		Resources::Shader* shader;
		GLuint FBO;

	private:

		GLuint _VBO;
		GLuint _RBO;
		GLuint _VAO;
		GLuint _Color;
	};
}