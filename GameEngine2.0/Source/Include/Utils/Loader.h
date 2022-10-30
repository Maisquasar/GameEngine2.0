#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Include/Math/Math.h"
#include "Include/Debug/Log.h"

namespace Utils::Loader
{
	void SkipLine(const char* data, uint32_t& pos);
	std::string GetLine(const char* data, uint32_t& pos);
	std::string GetString(std::string);
	int GetInt(const char* data, uint32_t& pos, int dec);
	float GetFloat(std::string line);
	Math::Vector4 GetVec4(std::string);
	Math::Vector3 GetVec3(std::string);
	Math::Vector2 GetVec2(std::string);
	std::vector<Math::Integer3> GetIndices(std::string);
	Math::Matrix4 GetMat4(const char* data, uint32_t& pos, int dec);

	const char* ReadFile(const char* filename, uint32_t& size, bool& success);
}