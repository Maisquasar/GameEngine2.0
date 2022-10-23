#pragma once
#include "Quaternion.h"
#include "../Math.h"

using namespace Math;

inline Quaternion Quaternion::operator+(const Quaternion& a)
{
	return Quaternion(x + a.x, y + a.y, z + a.z, w + a.w);
}

inline Quaternion Quaternion::operator-(const Quaternion& a)
{
	return Quaternion(x - a.x, y - a.y, z - a.z, w - a.w);
}

inline Quaternion Quaternion::operator*(const Quaternion& a)
{
	return Quaternion(w * a.x + x * a.w + y * a.z - z * a.y, w * a.y - x * a.z + y * a.w + z * a.x, w * a.z + x * a.y - y * a.x + z * a.w, w * a.w - x * a.x - y * a.y - z * a.z);
}

inline Math::Quaternion Math::Quaternion::AngleAxis(float angle, Vector3 axis)
{
	Quaternion q;
	float rad = Math::ToRadians(angle);
	q.w = cosf(rad / 2);
	q.x = sinf(rad / 2) * axis.x;
	q.y = sinf(rad / 2) * axis.y;
	q.z = sinf(rad / 2) * axis.z;
	return q;
}

inline Math::Vector3 Math::Quaternion::ToEuler()
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

inline Math::Matrix4 Math::Quaternion::ToRotationMatrix()
{
	Math::Matrix4 m = Math::Matrix4::Identity();
	m.at(0, 0) = 1 - 2 * pow(y, 2) - 2 * pow(z, 2);
	m.at(1, 0) = 2 * x * y + 2 * w * z;
	m.at(2, 0) = 2 * x * z + 2 * w * y;

	m.at(0, 1) = 2 * x * y + 2 * w * z;
	m.at(1, 1) = 1 - 2 * pow(x, 2) - 2 * pow(z, 2);
	m.at(2, 1) = 2 * y * z - 2 * w * x;

	m.at(0, 2) = 2 * x * z - 2 * w * y;
	m.at(1, 2) = 2 * y * z + 2 * w * x;
	m.at(2, 2) = 1 - 2 * pow(x, 2) - 2 * pow(y, 2);
	return m;
}