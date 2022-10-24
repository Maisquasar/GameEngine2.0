#pragma once
#include <GLFW/glfw3.h>
#include "Include/Debug/Log.h"
#include <glad/glad.h>
#include "IResource.h"

namespace Resources {
	static int TextureData = 0;
	class Texture : public Resources::IResource
	{
	public:
		Texture();
		~Texture();

		void NewTexture(std::string filename);

		void Load(std::string filename) override;

		int GetIndex() { return _index; }
		GLuint GetData() { return _textureData; }
		GLuint* GetDataPtr() { return &_textureData; }
		GLuint GetSampler() { return _sampler; }
		GLuint* GetSamplerPtr() { return &_sampler; }
	private:
		std::string _name;
		unsigned char* _data;
		GLuint _textureData = 0;
		GLuint _sampler;
		int _width;
		int _height;
		int _index;
	};
}