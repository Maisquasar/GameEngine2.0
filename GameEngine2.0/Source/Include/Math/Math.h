#pragma once
#include "Vector/Vector2.h"
#include "Vector/Vector3.h"
#include "Vector/Vector4.h"
#include "Integer/Integer2.h"
#include "Integer/Integer3.h"
#include "Matrix/Matrix4.h"
#include "Matrix/Quaternion.h"

#define PI 3.141592653f

namespace Math
{
	// Return the given angular value in degrees converted to radians
	inline float ToRadians(float in);

	// Return the given angular value in radians converted to degrees
	inline float ToDegrees(float in);

	inline float Cut(float in, float min, float max);

	inline int CutInt(int in, int min, int max);

	inline float Mod(float in, float value);

	inline float Min(float a, float b);

	inline float Max(float a, float b);

	inline bool IsEqual(float a, float b, float prec);

	inline bool IsEqualVec4(Vector4 a, Vector4 b, float prec);

	inline float EdgeFunction(const Integer2 p, const Integer2 a, const Integer2 b);

	inline bool IsTopLeft(const Integer2& a, const Integer2& b);

	inline Vector3 GetSphericalCoords(float r, float theta, float phi);
}