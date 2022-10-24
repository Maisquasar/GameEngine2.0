#include "Include/Math/Math.h"

float Math::ToRadians(float in)
{
	return in / 180.f * PI;
}

float Math::ToDegrees(float in)
{
	return in * 180.f / PI;
}

float Math::Cut(float in, float min, float max)
{
	if (in < min)
		in = min;
	if (in > max)
		in = max;
	return in;
}

int Math::CutInt(int in, int min, int max)
{
	if (in < min)
		in = min;
	if (in > max)
		in = max;
	return in;
}

float Math::Mod(float in, float value)
{
	return in - value * floorf(in / value);
}

float Math::Min(float a, float b)
{
	if (a > b)
		return b;
	return a;
}

float Math::Max(float a, float b)
{
	if (a > b)
		return a;
	return b;
}

bool Math::IsEqual(float a, float b, float prec)
{
	return (a - prec < b&& a + prec > b);
}

bool Math::IsEqualVec4(Vector4 a, Vector4 b, float prec)
{
	return (IsEqual(b.x, a.x, prec) && IsEqual(b.y, a.y, prec) && IsEqual(b.z, a.z, prec) && IsEqual(b.w, a.w, prec));
}

float Math::EdgeFunction(const Integer2 p, const Integer2 a, const Integer2 b)
{
	return (float)(p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

bool Math::IsTopLeft(const Integer2& a, const Integer2& b)
{
	return (a.y > b.y || (a.y == b.y && a.x < b.x));
}

Math::Vector3 Math::GetSphericalCoords(float r, float theta, float phi)
{
	return { r * sinf(theta) * cosf(phi),
			r * cosf(theta),
			r * sinf(theta) * sinf(phi) };
}