#include "stdafx.h"
#include "Include/Resources/Texture.h"
#include "Include/App.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB_Image/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <STB_Image/stb_image_write.h>

Resources::Texture::Texture() {}

Resources::Texture::~Texture() {}

void Resources::Texture::NewTexture(std::string filename)
{
	_index = Application.GetResourceManager()->TextureIndex;
	Application.GetResourceManager()->TextureIndex++;
	this->_name = filename;
	this->_initialized = true;
}

void Resources::Texture::Load(std::string filename)
{
#if MULTITHREAD_LOADING
	Application.ThreadManager.QueueJob(&Texture::MultiThreadLoading, this, filename, false);
#else
	MultiThreadLoading(filename);
#endif
}

void Resources::Texture::Load(std::string filename, bool shouldFlip)
{
#if MULTITHREAD_LOADING
	Application.ThreadManager.QueueJob(&Texture::MultiThreadLoading, this, filename, shouldFlip);
#else
	MultiThreadLoading(filename, shouldFlip);
#endif
}

void Resources::Texture::Write()
{
	stbi_write_png(GetPath().c_str(), _width, _height, 4, _data, _width * 4);
}

void Resources::Texture::LoadFromMemory(unsigned char* data, int len)
{
	int width, height, n;
	auto _data = stbi_loadf_from_memory(data, len, &width, &height, &n, 4);
	if (_data == nullptr) {
		auto fail = stbi_failure_reason();
		LOG(Debug::LogType::L_ERROR, "Can't load Texture: \"%s\" : %s", _path.c_str(), fail);
		return;
	}
	Loaded = true;
	glActiveTexture(GL_TEXTURE0 + _index);
	glGenTextures(1, &_textureData);
	glBindTexture(GL_TEXTURE_2D, _textureData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(_data);
	LOG(Debug::LogType::INFO, "Successfully loaded Texture: %s With Id : %d", _path.c_str(), _index);
	_initialized = true;
}

void Resources::Texture::MultiThreadLoading(std::string filename, bool flip)
{
	stbi_set_flip_vertically_on_load_thread(flip);
	_index = Application.GetResourceManager()->TextureIndex;
	Application.GetResourceManager()->TextureIndex++;
	int NrChannels;
	this->_data = stbi_load(_path.c_str(), &_width, &_height, &NrChannels, 4);
	Loaded = true;
	if (_data == nullptr) {
		auto fail = stbi_failure_reason();
		LOG(Debug::LogType::L_ERROR, "Can't load Texture: \"%s\" : %s", _path.c_str(), fail);
		return;
	}
#if !MULTITHREAD_LOADING
	Initialize();
#endif
}

void Resources::Texture::Initialize()
{
	glActiveTexture(GL_TEXTURE0 + _index);
	glGenTextures(1, &_textureData);
	glBindTexture(GL_TEXTURE_2D, _textureData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(_data);
	LOG(Debug::LogType::INFO, "Successfully loaded Texture: %s With Id : %d", _path.c_str(), _index);
	_initialized = true;
}
