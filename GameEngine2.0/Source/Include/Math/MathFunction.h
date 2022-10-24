#pragma once
#include <cmath>
#define PI 3.141592653f

namespace Math
{
	class Vector4;
	class Integer2;
	class Vector3;
	// Return the given angular value in degrees converted to radians
	float ToRadians(float in);

	// Return the given angular value in radians converted to degrees
	float ToDegrees(float in);

	float Cut(float in, float min, float max);

	int CutInt(int in, int min, int max);

	float Mod(float in, float value);

	float Min(float a, float b);

	float Max(float a, float b);

	bool IsEqual(float a, float b, float prec);

	bool IsEqualVec4(Vector4 a, Vector4 b, float prec);

	float EdgeFunction(const Integer2 p, const Integer2 a, const Integer2 b);

	bool IsTopLeft(const Integer2& a, const Integer2& b);

	Vector3 GetSphericalCoords(float r, float theta, float phi);
}