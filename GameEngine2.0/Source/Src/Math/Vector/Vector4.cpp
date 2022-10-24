#include "Include/Math/Math.h"
using namespace Math;

Math::Vector4::Vector4(const Vector3& in, float wIn) : x(in.x), y(in.y), z(in.z), w(wIn) {}

Vector3 Math::Vector4::GetVector() const
{
	return Vector3(x, y, z);
}

Math::Vector4 Math::Vector4::Homogenize() const
{
	return Vector4(GetVector() / w);
}

Math::Vector4 Math::Vector4::UnitVector()
{
	Vector4 homogenized = Homogenize();
	return homogenized / homogenized.GetVector().GetLength();
}

float Math::Vector4::LengthSquared() const
{
	return Homogenize().GetVector().LengthSquared();
}

float Math::Vector4::GetLength() const
{
	return sqrtf(LengthSquared());
}

Math::Vector4 Math::Vector4::operator+(const Vector4& a) const
{
	return Vector4(x + a.x, y + a.y, z + a.z, w + a.w);
}

Math::Vector4 Math::Vector4::operator-(const Vector4& a) const
{
	return Vector4(x - a.x, y - a.y, z - a.z, w - a.w);
}

Math::Vector4 Math::Vector4::operator*(const Vector4& a) const
{
	return Vector4(x * a.x, y * a.y, z * a.z, w * a.w);
}

Math::Vector4 Math::Vector4::operator*(const float& a) const
{
	return Vector4(this->x * a, this->y * a, this->z * a, this->w * a);
}
Math::Vector4 Math::Vector4::operator/(const float& b) const
{
	if (b == 0.0f)
		return operator*(VEC_HIGH_VALUE);
	Vector4 res = operator*(1 / b);
	return res;
}

bool Math::Vector4::operator==(const Vector4& b)
{
	return (x == b.x && y == b.y && z == b.z && w == b.w);
}

float& Math::Vector4::operator[](const size_t a)
{
	return *((&x) + a);
}

const float& Math::Vector4::operator[](const size_t a) const
{
	return *((&x) + a);
}

bool Math::Vector4::IsCollinearWith(Vector4 a)
{
	float res = DotProduct(a);
	return (res < VEC_COLLINEAR_PRECISION);
}

float Math::Vector4::DotProduct(Vector4 a)
{
	return (a.x * x + a.y * y + a.z * z);
}

Math::Vector4 Math::Vector4::CrossProduct(Vector4 a)
{
	return Vector4((y * a.z) - (z * a.y), (z * a.x) - (x * a.z), (x * a.y) - (y * a.x), 1.0f);
}

Math::Vector4 Math::Vector4::Negate()
{
	return operator*(-1);
}

bool Math::Vector4::IsIntEquivalent(Vector4 a)
{
	return ((int)x == (int)a.x && (int)y == a.y && (int)z == (int)a.z && (int)w == (int)a.w);
}

void Math::Vector4::Print()
{
	printf("(%.2f, %.2f, %.2f, %.2f)\n", x, y, z, w);
}

std::string Math::Vector4::ToString()
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w);
}