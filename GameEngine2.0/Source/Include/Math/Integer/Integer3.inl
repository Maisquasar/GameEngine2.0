#pragma once
#include "Integer3.h"
#include "../Vector/Vector3.h"

using namespace Math;

inline Math::Integer3::Integer3(const Vector3& in) : x((int)in.x), y((int)in.y), z((int)in.z) {}

inline Math::Integer3 Math::Integer3::operator+(const Integer3& a)
{
	return Integer3(a.x + this->x, a.y + this->y, a.z + this->z);
}

inline Math::Integer3 Math::Integer3::operator-(const Integer3& a)
{
	return Integer3(x - a.x, y - a.y, z - a.z);
}

inline Math::Integer3 Math::Integer3::operator*(const Integer3& a)
{
	Integer3 res = Integer3(this->x * a.x, this->y * a.y, this->z * a.z);
	return res;
}

inline Math::Integer3 Math::Integer3::operator*(const float& a)
{
	Integer3 res = Integer3(this->x * (int)a, this->y * (int)a, this->z * (int)a);
	return res;
}

inline Math::Integer3 Math::Integer3::operator/(const float& a)
{
	if (a == 0)
		return Integer3(INT32_MAX, INT32_MAX, INT32_MAX);
	Integer3 res = Integer3(x / (int)a, y / (int)a, z / (int)a);
	return res;
}

inline bool Math::Integer3::operator==(const Integer3& b)
{
	return (x == b.x && y == b.y && z == b.z);
}

inline int& Math::Integer3::operator[](const size_t a)
{
	return *((&x) + a);
}