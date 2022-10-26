#include "Include/Math/Math.h"
using namespace Math;

Quaternion Quaternion::operator+(const Quaternion& a)
{
	return Quaternion(x + a.x, y + a.y, z + a.z, w + a.w);
}

Quaternion Quaternion::operator-(const Quaternion& a)
{
	return Quaternion(x - a.x, y - a.y, z - a.z, w - a.w);
}

Quaternion Quaternion::operator*(const Quaternion& a)
{
	return Quaternion(w * a.x + x * a.w + y * a.z - z * a.y, w * a.y - x * a.z + y * a.w + z * a.x, w * a.z + x * a.y - y * a.x + z * a.w, w * a.w - x * a.x - y * a.y - z * a.z);
}

Vector3 Math::Quaternion::operator*(const Vector3& a)
{
	Vector3 vector;
	float num = this->x * 2.f;
	float num2 = this->y * 2.f;
	float num3 = this->z * 2.f;
	float num4 = this->x * num;
	float num5 = this->y * num2;
	float num6 = this->z * num3;
	float num7 = this->x * num2;
	float num8 = this->x * num3;
	float num9 = this->y * num3;
	float num10 = this->w * num;
	float num11 = this->w * num2;
	float num12 = this->w * num3;
	vector.x = (((1.f - (num5 + num6)) * a.x) + ((num7 - num12) * a.y)) + ((num8 + num11) * a.z);
	vector.y = (((num7 + num12) * a.x) + ((1.f - (num4 + num6)) * a.y)) + ((num9 - num10) * a.z);
	vector.z = (((num8 - num11) * a.x) + ((num9 + num10) * a.y)) + ((1.f - (num4 + num5)) * a.z);
	return vector;
}

Math::Quaternion Math::Quaternion::AngleAxis(float angle, Vector3 axis)
{
	Quaternion q;
	float rad = Math::ToRadians(angle);
	q.w = cosf(rad / 2);
	q.x = sinf(rad / 2) * axis.x;
	q.y = sinf(rad / 2) * axis.y;
	q.z = sinf(rad / 2) * axis.z;
	return q;
}

Math::Vector3 Math::Quaternion::ToEuler()
{
	Math::Vector3 euler;
	Math::Matrix4 rotMat = ToRotationMatrix();

	// Get Rotation from rotation matrix.
	float thetaX, thetaY, thetaZ;
	if (rotMat.at(2, 1) < 1)
	{
		if (rotMat.at(2, 1) > -1)
		{
			thetaX = asin(-rotMat.at(2, 1));
			thetaY = atan2(rotMat.at(2, 0), rotMat.at(2, 2));
			thetaZ = atan2(rotMat.at(0, 1), rotMat.at(1, 1));
		}
		else
		{
			thetaX = (float)PI / 2;
			thetaY = -atan2(rotMat.at(1, 0), rotMat.at(0, 0));
			thetaZ = 0;
		}
	}
	else
	{
		thetaX = -PI / 2;
		thetaY = atan2(rotMat.at(1, 0), rotMat.at(0, 0));
		thetaZ = 0;
	}
	return Vector3(thetaX, thetaY, thetaZ).ToDeegree();
}

Math::Matrix4 Math::Quaternion::ToRotationMatrix()
{
	Math::Matrix4 m = Math::Matrix4::Identity();
	m.at(0, 0) = 1 - 2 * pow(y, 2.f) - 2 * pow(z, 2.f);
	m.at(1, 0) = 2 * x * y + 2 * w * z;
	m.at(2, 0) = 2 * x * z + 2 * w * y;

	m.at(0, 1) = 2 * x * y + 2 * w * z;
	m.at(1, 1) = 1 - 2 * pow(x, 2.f) - 2 * pow(z, 2.f);
	m.at(2, 1) = 2 * y * z - 2 * w * x;

	m.at(0, 2) = 2 * x * z - 2 * w * y;
	m.at(1, 2) = 2 * y * z + 2 * w * x;
	m.at(2, 2) = 1 - 2 * pow(x, 2.f) - 2 * pow(y, 2.f);
	return m;
}

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