#pragma once
#include "Math.h"

inline float Math::ToRadians(float in)
{
	return in / 180.f * PI;
}

inline float Math::ToDegrees(float in)
{
	return in * 180.f / PI;
}

inline float Math::Cut(float in, float min, float max)
{
	if (in < min)
		in = min;
	if (in > max)
		in = max;
	return in;
}

inline int Math::CutInt(int in, int min, int max)
{
	if (in < min)
		in = min;
	if (in > max)
		in = max;
	return in;
}

inline float Math::Mod(float in, float value)
{
	return in - value * floorf(in / value);
}

inline float Math::Min(float a, float b)
{
	if (a > b)
		return b;
	return a;
}

inline float Math::Max(float a, float b)
{
	if (a > b)
		return a;
	return b;
}

inline bool Math::IsEqual(float a, float b, float prec)
{
	return (a - prec < b&& a + prec > b);
}

inline bool Math::IsEqualVec4(Vector4 a, Vector4 b, float prec)
{
	return (IsEqual(b.x, a.x, prec) && IsEqual(b.y, a.y, prec) && IsEqual(b.z, a.z, prec) && IsEqual(b.w, a.w, prec));
}

inline float Math::EdgeFunction(const Integer2 p, const Integer2 a, const Integer2 b)
{
	return (float)(p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

inline bool Math::IsTopLeft(const Integer2& a, const Integer2& b)
{
	return (a.y > b.y || (a.y == b.y && a.x < b.x));
}

inline Math::Vector3 Math::GetSphericalCoords(float r, float theta, float phi)
{
	return { r * sinf(theta) * cosf(phi),
			r * cosf(theta),
			r * sinf(theta) * sinf(phi) };
}