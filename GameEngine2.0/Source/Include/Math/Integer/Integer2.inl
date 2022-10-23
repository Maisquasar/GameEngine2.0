#pragma once
#include "../Math.h"

using namespace Math;

inline Math::Integer2::Integer2(const Vector2& in) : x((int)in.x), y((int)in.y) {}

inline Math::Integer2 Math::Integer2::operator+(const Integer2& a)
{
	Integer2 res = Integer2(a.x + this->x, a.y + this->y);
	return res;
}

inline Math::Integer2 Math::Integer2::operator-(const Integer2& a)
{
	return Integer2(x - a.x, y - a.y);
}

inline Math::Integer2 Math::Integer2::operator*(const Integer2& a)
{
	Integer2 res = Integer2(this->x * a.x, this->y * a.y);
	return res;
}

inline Math::Integer2 Math::Integer2::operator*(const float& a)
{
	Integer2 res = Integer2(this->x * (int)a, this->y * (int)a);
	return res;
}

inline Math::Integer2 Math::Integer2::operator/(const float& a)
{
	if (a == 0)
		return Integer2(INT32_MAX, INT32_MAX);
	Integer2 res = Integer2(x / (int)a, y / (int)a);
	return res;
}

inline bool Math::Integer2::operator==(const Integer2& b)
{
	return (x == b.x && y == b.y);
}