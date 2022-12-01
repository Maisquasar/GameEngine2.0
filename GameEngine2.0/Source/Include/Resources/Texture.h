#pragma once
#include <GLFW/glfw3.h>
#include "Include/Debug/Log.h"
#include <glad/glad.h>
#include "IResource.h"

namespace Resources {
	class Texture : public BaseResource<Texture>
	{
	public:
		Texture();
		~Texture();

		void NewTexture(std::string filename);

		void Load(std::string filename) override;

		void MultiThreadLoading(std::string filename);

		void Initialize() override;

		int GetIndex() { return _index; }
		GLuint GetData() { return _textureData; }
		GLuint* GetDataPtr() { return &_textureData; }
		GLuint GetSampler() { return _sampler; }
		GLuint* GetSamplerPtr() { return &_sampler; }
	private:
		unsigned char* _data;
		GLuint _textureData = 0;
		GLuint _sampler;
		int _width;
		int _height;
		int _index;
	};
}