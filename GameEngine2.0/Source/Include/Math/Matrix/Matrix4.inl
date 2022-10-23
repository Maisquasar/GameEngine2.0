#pragma once
#include "Matrix4.h"
using namespace Math;

inline float& Matrix4::operator[](const size_t in)
{
	return content[in / 4][in % 4];
}

inline float& Matrix4::at(const unsigned char x, const unsigned char y)
{
	if (x > 3 || y > 3)
		return content[0][0];
	return content[y][x];
}
