#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
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

	void SortByDistanceFromCamera(std::vector<Core::Node*>& list, Math::Vector3 CameraPosition);

	std::vector<std::pair<Core::Node*, float> > BubbleSort(std::map<Core::Node*, float>& list);
}