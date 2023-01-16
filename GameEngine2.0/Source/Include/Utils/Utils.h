#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include <cstdlib>

#include "Include/Math/Math.h"

namespace Core
{
	class Node;
}

namespace Utils
{
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

	// Return the distance between the two points with the axis given.
	float GetDistanceAxis(Math::Vec3 point1, Math::Vec3 point2, Math::Vec3 axis);

	void SortByDistanceFromCamera(std::vector<Core::Node*>& list, Math::Vec3 CameraPosition, Math::Vec3 Forward);

	std::vector<std::pair<Core::Node*, float> > BubbleSort(std::map<Core::Node*, float>& list);

	void GetPlaneVertices(std::vector<Math::Vec3> &Positions, std::vector<Math::Vec2>& TextureUVs, std::vector<Math::Vec3> &Normals, std::vector<Math::IVec3>& Indices);

	float RandomFloat(float min, float max);

	bool InputTextAutoCompletion(std::vector<std::string> datas, std::string& result);
}