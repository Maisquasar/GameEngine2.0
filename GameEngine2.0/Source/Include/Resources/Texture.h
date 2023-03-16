#pragma once
#include "Include/Debug/Log.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "IResource.h"

namespace Resources {
	class Texture : public BaseResource<Texture>
	{
	public:
		Texture();
		~Texture();

		void NewTexture(std::string filename);

		void Load() override;

		void Load(bool shouldFlip);

		void Write();

		void LoadFromMemory(unsigned char* data, int len);

		void MultiThreadLoading(bool shouldFlip);

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