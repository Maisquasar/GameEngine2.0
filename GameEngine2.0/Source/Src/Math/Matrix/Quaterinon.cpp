#include "Include/Math/Math.h"

float Math::Quaternion::Dot(Math::Quaternion a)
{
	return x * a.x + y * a.y + z * a.z + w * a.w;
}

void Math::Quaternion::Normalize()
{
	*this = GetNormalized();
}

Math::Quaternion Math::Quaternion::GetNormalized()
{
	float mag = sqrtf(Dot(*this));

	if (mag < FLT_MIN)
		return Quaternion::Identity();
	else
		return Quaternion(x / mag, y / mag, z / mag, w / mag);
}

void Math::Quaternion::Inverse()
{
	*this = GetInverse();
}

Math::Quaternion Math::Quaternion::GetInverse()
{
	float d = pow(w, 2.f) + pow(x, 2.f) + pow(y, 2.f) + pow(z, 2.f);
	if (x == 0 && y == 0 && z == 0 && w == 0)
		return *this;
	else
		return Quaternion(-x / d, -y / d, -z / d, w / d);
}

void Math::Quaternion::Print() const
{
	printf("Quaternion { %f, %f, %f, %f}\n", x, y, z, w);
}