#pragma once
#include "Include/Math/Integer/Integer2.h"
#include "Include/Math/Vector/Vector2.h"
using namespace Math;

Integer2::Integer2(const Vector2& in) : x((int)in.x), y((int)in.y) {}

Integer2 Integer2::operator+(const Integer2& a)
{
	Integer2 res = Integer2(a.x + this->x, a.y + this->y);
	return res;
}

Integer2 Integer2::operator-(const Integer2& a)
{
	return Integer2(x - a.x, y - a.y);
}

Integer2 Integer2::operator*(const Integer2& a)
{
	Integer2 res = Integer2(this->x * a.x, this->y * a.y);
	return res;
}

Integer2 Integer2::operator*(const float& a)
{
	Integer2 res = Integer2(this->x * (int)a, this->y * (int)a);
	return res;
}

Integer2 Integer2::operator/(const float& a)
{
	if (a == 0)
		return Integer2(INT32_MAX, INT32_MAX);
	Integer2 res = Integer2(x / (int)a, y / (int)a);
	return res;
}

bool Integer2::operator==(const Integer2& b)
{
	return (x == b.x && y == b.y);
}