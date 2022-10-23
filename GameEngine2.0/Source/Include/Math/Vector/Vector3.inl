#pragma once
#include "../Math.h"

using namespace Math;

inline Vector3::Vector3(Vector4 a) : x(a.x), y(a.y), z(a.z) {}

inline float Vector3::LengthSquared() const
{
	return (x * x + y * y + z * z);
}

inline float Vector3::GetLength() const
{
	return sqrtf(LengthSquared());
}

inline Vector3 Vector3::operator=(const Vector4& b) const
{
	return Vector3(b.x, b.y, b.z);
}
inline Vector3 Vector3::operator=(Vector4 b)
{
	return Vector3(b.x, b.y, b.z);
}
inline Vector3 Vector3::operator=(Integer3 b)
{
	return Vector3(b.x, b.y, b.z);
}

inline Vector3 Vector3::operator=(const float b[3])
{
	return Vector3(b[0], b[1], b[2]);
}

inline Vector3 Vector3::operator+(const Vector3& a) const
{
	Vector3 res = Vector3(a.x + this->x, a.y + this->y, a.z + this->z);
	return res;
}

inline void Vector3::operator+=(const Vector3& a)
{
	this->x += a.x;
	this->y += a.y;
	this->z += a.z;
}

inline void Vector3::operator-=(const Vector3& a)
{
	this->x -= a.x;
	this->y -= a.y;
	this->z -= a.z;
}

inline Vector3 Vector3::operator-(const Vector3& a) const
{
	Vector3 res = Vector3(this->x - a.x, this->y - a.y, this->z - a.z);
	return res;
}

inline Vector3 Vector3::operator*(const Vector3& a) const
{
	Vector3 res = Vector3(this->x * a.x, this->y * a.y, this->z * a.z);
	return res;
}

inline Vector3 Vector3::operator*(const Integer3& a) const
{
	Vector3 res = Vector3(this->x * a.x, this->y * a.y, this->z * a.z);
	return res;
}

inline Vector3 Vector3::operator*(const float& a) const
{
	Vector3 res = Vector3(this->x * a, this->y * a, this->z * a);
	return res;
}

inline Vector3 Vector3::operator/(const float& a) const
{
	if (a == 0.0f)
		return operator*(VEC_HIGH_VALUE);
	Vector3 res = operator*(1 / a);
	return res;
}

inline bool Vector3::operator==(const Vector3& b) const
{
	return (x == b.x && y == b.y && z == b.z);
}

inline bool Vector3::operator>(const Vector3& b) const
{
	return (x > b.x || y > b.y || z > b.z);
}

inline bool Vector3::operator<(const Vector3& b) const
{
	return (x < b.x || y < b.y || z < b.z);
}

inline bool Vector3::operator!=(const Vector3& b) const
{
	return (x != b.x || y != b.y || z != b.z);
}

inline float& Vector3::operator[](const size_t a)
{
	return *((&x) + a);
}

inline const float& Vector3::operator[](const size_t a) const
{
	return *((&x) + a);
}

inline float Vector3::DotProduct(Vector3 a)
{
	return (a.x * x + a.y * y + a.z * z);
}

inline bool Vector3::IsCollinearWith(Vector3 a)
{
	float res = this->DotProduct(a);
	return (res < VEC_COLLINEAR_PRECISION);
}

inline float Vector3::GetDistanceBetween(Vector3 a)
{
	float i = a.x - x;
	float j = a.y - y;
	float h = a.z - z;
	return sqrt(i * i + j * j + h * h);
}


inline Vector3 Vector3::CrossProduct(Vector3 a)
{
	return Vector3((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x));
}

inline Vector3 Vector3::UnitVector() const
{
	return operator/(GetLength());
}

inline Vector3 Vector3::Negate()
{
	return operator*(-1);
}

inline bool Vector3::IsIntEquivalent(Vector3 a)
{
	return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z);
}


// Convert degree to rad
inline Vector3 Vector3::ToRad()
{
	return Vector3(ToRadians(x), ToRadians(y), ToRadians(z));
}

// Convert rad to Degree
inline Vector3 Vector3::ToDeegree()
{
	return Vector3(ToDegrees(x), ToDegrees(y), ToDegrees(z));
}

inline Vector3 Vector3::Clamp(Vector3 min, Vector3 max)
{
	Vector3 tmp = *this;
	if (tmp < min)
		tmp = min;
	else if (tmp > max)
		tmp = max;
	return tmp;
}

inline Vector3 Vector3::ClampMagnitude(Vector3 vector, float maxLength)
{
	float sqrmag = vector.LengthSquared();
	if (sqrmag > maxLength * maxLength)
	{
		float mag = (float)sqrtf(sqrmag);
		float normalized_x = vector.x / mag;
		float normalized_y = vector.y / mag;
		float normalized_z = vector.z / mag;
		return Vector3(normalized_x * maxLength,
			normalized_y * maxLength,
			normalized_z * maxLength);
	}
	return vector;
}

inline Quaternion  Vector3::ToQuaternion()
{
	auto q = Quaternion::AngleAxis(x, Vector3(1, 0, 0))
		* Quaternion::AngleAxis(y, Vector3(0, 1, 0))
		* Quaternion::AngleAxis(z, Vector3(0, 0, 1));

	return q;
}