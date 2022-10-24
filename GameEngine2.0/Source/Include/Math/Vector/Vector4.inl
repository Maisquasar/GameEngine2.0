#include "../MathFunction.h"
#include "Vector4.h"
#include "Vector3.h"

using namespace Math;

inline Math::Vector4::Vector4(const Vector3& in, float wIn = 1.0f) : x(in.x), y(in.y), z(in.z), w(wIn) {}

inline Vector3 Math::Vector4::GetVector() const
{
	return Vector3(x, y, z);
}

inline Math::Vector4 Math::Vector4::Homogenize() const
{
	return Vector4(GetVector() / w);
}

inline Math::Vector4 Math::Vector4::UnitVector()
{
	Vector4 homogenized = Homogenize();
	return homogenized / homogenized.GetVector().GetLength();
}

inline float Math::Vector4::LengthSquared() const
{
	return Homogenize().GetVector().LengthSquared();
}

inline float Math::Vector4::GetLength() const
{
	return sqrtf(LengthSquared());
}

inline Math::Vector4 Math::Vector4::operator+(const Vector4& a) const
{
	return Vector4(x + a.x, y + a.y, z + a.z, w + a.w);
}

inline Math::Vector4 Math::Vector4::operator-(const Vector4& a) const
{
	return Vector4(x - a.x, y - a.y, z - a.z, w - a.w);
}

inline Math::Vector4 Math::Vector4::operator*(const Vector4& a) const
{
	return Vector4(x * a.x, y * a.y, z * a.z, w * a.w);
}

inline Math::Vector4 Math::Vector4::operator*(const float& a) const
{
	return Vector4(this->x * a, this->y * a, this->z * a, this->w * a);
}
inline Math::Vector4 Math::Vector4::operator/(const float& b) const
{
	if (b == 0.0f)
		return operator*(VEC_HIGH_VALUE);
	Vector4 res = operator*(1 / b);
	return res;
}

inline bool Math::Vector4::operator==(const Vector4& b)
{
	return (x == b.x && y == b.y && z == b.z && w == b.w);
}

inline float& Math::Vector4::operator[](const size_t a)
{
	return *((&x) + a);
}

inline const float& Math::Vector4::operator[](const size_t a) const
{
	return *((&x) + a);
}

inline bool Math::Vector4::IsCollinearWith(Vector4 a)
{
	float res = DotProduct(a);
	return (res < VEC_COLLINEAR_PRECISION);
}

inline float Math::Vector4::DotProduct(Vector4 a)
{
	return (a.x * x + a.y * y + a.z * z);
}

inline Math::Vector4 Math::Vector4::CrossProduct(Vector4 a)
{
	return Vector4((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x), 1.0f);
}

inline Math::Vector4 Math::Vector4::Negate()
{
	return operator*(-1);
}

inline bool Math::Vector4::IsIntEquivalent(Vector4 a)
{
	return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z && (int)w == (int)a.w);
}