#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Include/Math/Math.h"
#include "Include/Debug/Log.h"

namespace Resources
{
	class Material;
}

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

	void MtlLoader(std::string path);

	void WriteMaterial(Resources::Material* material);

	void LoadMaterial(std::string path);

	template<typename ... Args>
	std::string StringFormat(const std::string& format, Args ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
		auto size = static_cast<size_t>(size_s);
		std::unique_ptr<char[]> buf(new char[size]);
		std::snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}
}