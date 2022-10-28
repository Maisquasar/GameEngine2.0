#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Include/Math/Math.h"
#include "Include/Debug/Log.h"

namespace Utils::Loader
{
	void SkipLine(const char* data, uint32_t& pos);
	std::string GetString(const char* data, uint32_t& pos, int dec);
	int GetInt(const char* data, uint32_t& pos, int dec);
	float GetFloat(const char* data, uint32_t& pos, int dec);
	Math::Vector4 GetVec4(const char* data, uint32_t& pos, int dec);
	Math::Vector3 GetVec3(const char* data, uint32_t& pos, int dec);
	Math::Vector2 GetVec2(const char* data, uint32_t& pos, int dec);
	Math::Matrix4 GetMat4(const char* data, uint32_t& pos, int dec);

	const char* ReadFile(const char* filename, uint32_t& size, bool& success);
}