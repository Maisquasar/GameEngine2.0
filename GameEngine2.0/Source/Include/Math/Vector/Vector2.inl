#pragma once
#include "Vector2.h"
#include "../Integer/Integer2.h"
using namespace Math;

inline Math::Vector2::Vector2(const Integer2& a) : x(a.x), y(a.y){}

inline float Vector2::LengthSquared()
{
	return (x * x + y * y);
}

inline float Vector2::GetLength()
{
	return sqrtf(LengthSquared());
}

inline Vector2 Vector2::operator+(const Vector2& a) const
{
	Vector2 res = Vector2(a.x + this->x, a.y + this->y);
	return res;
}

inline Vector2 Vector2::operator-(const Vector2& a) const
{
	Vector2 res = Vector2(this->x - a.x, this->y - a.y);
	return res;
}

inline Vector2 Vector2::operator*(const Vector2& a)const
{
	Vector2 res = Vector2(this->x * a.x, this->y * a.y);
	return res;
}

inline Vector2 Vector2::operator*(const float& a)const
{
	Vector2 res = Vector2(this->x * a, this->y * a);
	return res;
}

inline Vector2 Vector2::operator/(const float& a)const
{
	if (a == 0.0)
		return operator*(VEC_HIGH_VALUE);
	Vector2 res = operator*(1 / a);
	return res;
}

inline bool Vector2::operator==(const Vector2& b)const
{
	return (x == b.x && y == b.y);
}

inline bool Vector2::IsCollinearWith(Vector2 a)
{
	float res = a.x * y - a.y * x;
	return (res < VEC_COLLINEAR_PRECISION);
}

inline float Vector2::DotProduct(Vector2 a)
{
	return (a.x * x + a.y * y);
}

inline float Vector2::CrossProduct(Vector2 a)
{
	return (x * a.y - y * a.x);
}

inline Vector2 Vector2::UnitVector()
{
	return operator/(GetLength());
}

inline Vector2 Vector2::Negate()
{
	return operator*(-1);
}

inline Vector2 Vector2::GetNormal()
{
	return Vector2(-y, x);
}

inline bool Vector2::IsIntEquivalent(Vector2 a)
{
	return ((int)x == (int)a.x && (int)y == a.y);
}

inline float Vector2::GetDistanceFromPoint(Vector2 a)
{
	float i = a.x - x;
	float j = a.y - y;
	return sqrt(i * i + j * j);
}