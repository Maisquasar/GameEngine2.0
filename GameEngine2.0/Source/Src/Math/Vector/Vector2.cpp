#include <stdio.h>
#include "Include/Math/Vector/Vector2.h"
#include "Include/Math/Integer/Integer2.h"

using namespace Math;

Math::Vector2::Vector2(const Integer2& a) : x((float)a.x), y((float)a.y) {}

float Vector2::LengthSquared()
{
	return (x * x + y * y);
}

float Vector2::GetLength()
{
	return sqrtf(LengthSquared());
}

Vector2 Vector2::operator+(const Vector2& a) const
{
	Vector2 res = Vector2(a.x + this->x, a.y + this->y);
	return res;
}

Vector2 Vector2::operator-(const Vector2& a) const
{
	Vector2 res = Vector2(this->x - a.x, this->y - a.y);
	return res;
}

Vector2 Vector2::operator*(const Vector2& a)const
{
	Vector2 res = Vector2(this->x * a.x, this->y * a.y);
	return res;
}

Vector2 Vector2::operator*(const float& a)const
{
	Vector2 res = Vector2(this->x * a, this->y * a);
	return res;
}

Vector2 Vector2::operator/(const float& a)const
{
	if (a == 0.0)
		return operator*(VEC_HIGH_VALUE);
	Vector2 res = operator*(1 / a);
	return res;
}

bool Vector2::operator==(const Vector2& b)const
{
	return (x == b.x && y == b.y);
}

bool Vector2::IsCollinearWith(Vector2 a)
{
	float res = a.x * y - a.y * x;
	return (res < VEC_COLLINEAR_PRECISION);
}

float Vector2::DotProduct(Vector2 a)
{
	return (a.x * x + a.y * y);
}

float Vector2::CrossProduct(Vector2 a)
{
	return (x * a.y - y * a.x);
}

Vector2 Vector2::UnitVector()
{
	return operator/(GetLength());
}

Vector2 Vector2::Negate()
{
	return operator*(-1);
}

Vector2 Vector2::GetNormal()
{
	return Vector2(-y, x);
}

bool Vector2::IsIntEquivalent(Vector2 a)
{
	return ((int)x == (int)a.x && (int)y == a.y);
}

float Vector2::GetDistanceFromPoint(Vector2 a)
{
	float i = a.x - x;
	float j = a.y - y;
	return sqrtf(powf(i, 2.f) + powf(j, 2.f));
}

void Vector2::Print()
{
	printf("(%.2f, %.2f)\n", x, y);
}

std::string Vector2::ToString()
{
	return std::to_string(x) + " " + std::to_string(y);
}