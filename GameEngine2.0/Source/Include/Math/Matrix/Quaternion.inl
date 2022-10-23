#pragma once
#include "Quaternion.h"

using namespace Math;

inline Quaternion Quaternion::operator+(const Quaternion& a)
{
	return Quaternion(x + a.x, y + a.y, z + a.z, w + a.w);
}

inline Quaternion Quaternion::operator-(const Quaternion& a)
{
	return Quaternion(x - a.x, y - a.y, z - a.z, w - a.w);
}

inline Quaternion Quaternion::operator*(const Quaternion& a)
{
	return Quaternion(w * a.x + x * a.w + y * a.z - z * a.y, w * a.y - x * a.z + y * a.w + z * a.x, w * a.z + x * a.y - y * a.x + z * a.w, w * a.w - x * a.x - y * a.y - z * a.z);
}