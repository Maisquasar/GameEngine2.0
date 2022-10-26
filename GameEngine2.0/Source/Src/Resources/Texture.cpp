#include "Include/Resources/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB_Image/stb_image.h>

Resources::Texture::Texture() {}

Resources::Texture::~Texture() {}

void Resources::Texture::NewTexture(std::string filename)
{
	_index = TextureData;
	TextureData++;
	this->_name = filename;
}

void Resources::Texture::Load(std::string filename)
{
	_index = TextureData;
	TextureData++;
	this->_name = filename;
	int NrChannels;
	this->_data = stbi_load(_path.c_str(), &_width, &_height, &NrChannels, 4);
	if (_data == nullptr) {
		auto fail = stbi_failure_reason();
		LOG(Debug::LogType::L_ERROR, "Can't load Texture: \"%s\" : %s", _path.c_str(), fail);
		return;
	}
	this->Loaded = true;

	glActiveTexture(GL_TEXTURE0 + _index);
	glGenTextures(1, &_textureData);
	glBindTexture(GL_TEXTURE_2D, _textureData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(_data);
	LOG(Debug::LogType::INFO, "Successfully loaded Texture: %s With Id : %d", _path.c_str(), _index);
}
