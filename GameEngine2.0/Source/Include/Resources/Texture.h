#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "IResource.h"
#include "Include/Debug/Log.h"

namespace Resources {
	static int TextureData = 0;
	class Texture : public Resources::IResource
	{
	public:
		Texture();
		~Texture();

		void Load(const char* filename) override;

	private:
		const char* _name;
		unsigned char* _data;
		GLuint _textureData = 0;
		int _width;
		int _height;
		int _index;
	};
}