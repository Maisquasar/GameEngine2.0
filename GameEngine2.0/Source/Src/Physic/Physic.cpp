#include "Include/Physic/Physic.h"
#include "Include/Physic/Ray.h"
#include "Include/Core/Transform.h"
#include "Include/Core/Components/BoxCollider.h"
#include "Include/Render/Camera.h"

std::vector<float> Physic::GetCubeVertices()
{
	std::vector<float> vOut;
	std::vector<Math::Vec3> v;
	std::vector<Math::Vec3> n;
	v = {
		{ 1.000000, 1.000000, -1.000000},
		{ 1.000000, -1.000000, -1.000000},
		{ 1.000000, 1.000000, 1.000000},
		{ 1.000000, -1.000000, 1.000000},
		{ -1.000000, 1.000000, -1.000000},
		{ -1.000000, -1.000000, -1.000000},
		{ -1.000000, 1.000000, 1.000000},
		{ -1.000000, -1.000000, 1.000000},
	};
	n = {
		{0.0000, 1.0000, 0.0000},
		{0.0000, 0.0000, 1.0000},
		{-1.0000, 0.0000, 0.0000},
		{0.0000, -1.0000, 0.0000},
		{1.0000, 0.0000, 0.0000},
		{0.0000, 0.0000, -1.0000},
	};

	std::vector<int> indices{
	5, 1, 3, 1, 1 ,1,
	3, 2, 8, 2, 4 ,2,
	7, 3, 6, 3, 8 ,3,
	2, 4, 8, 4, 6 ,4,
	1, 5, 4, 5, 2 ,5,
	5, 6, 2, 6, 6 ,6,
	5, 1, 7, 1, 3 ,1,
	3, 2, 7, 2, 8 ,2,
	7, 3, 5, 3, 6 ,3,
	2, 4, 4, 4, 8 ,4,
	1, 5, 3, 5, 4 ,5,
	5, 6, 1, 6, 2 ,6,
	};

	for (int k = 0; k < indices.size(); k += 2)
	{
		indices[k] -= 1;
		indices[k + 1ll] -= 1;
		vOut.push_back(v[indices[k]].x);
		vOut.push_back(v[indices[k]].y);
		vOut.push_back(v[indices[k]].z);

		vOut.push_back(n[indices[k + 1ll]].x);
		vOut.push_back(n[indices[k + 1ll]].y);
		vOut.push_back(n[indices[k + 1ll]].z);
	}

	return vOut;
}

std::vector<float> Physic::GetSphereVertices(float radius)
{
	std::vector<float> vOut;
	std::vector<Math::Vec3> v;
	std::vector<Math::Vec3> n;
	float r = radius;
	int mLat = 20;
	int mLon = 20;
	for (int j = 0; j < mLat; ++j)
	{
		float theta0 = ((j + 0) / (float)mLat) * PI;
		float theta1 = ((j + 1) / (float)mLat) * PI;

		for (int i = 0; i < mLon; ++i)
		{
			float phi0 = ((i + 0) / (float)mLon) * 2.f * PI;
			float phi1 = ((i + 1) / (float)mLon) * 2.f * PI;

			Math::Vec3 c0 = Math::GetSphericalCoords(r, theta0, phi0);
			Math::Vec3 c1 = Math::GetSphericalCoords(r, theta0, phi1);
			Math::Vec3 c2 = Math::GetSphericalCoords(r, theta1, phi1);
			Math::Vec3 c3 = Math::GetSphericalCoords(r, theta1, phi0);

			v.push_back(c0);
			v.push_back(c1);
			v.push_back(c2);

			v.push_back(c0);
			v.push_back(c2);
			v.push_back(c3);

			n.push_back(c0.UnitVector());
			n.push_back(c1.UnitVector());
			n.push_back(c2.UnitVector());

			n.push_back(c0.UnitVector());
			n.push_back(c2.UnitVector());
			n.push_back(c3.UnitVector());
		}
	}

	for (int k = 0; k < v.size(); k++)
	{
		vOut.push_back(v[k].x);
		vOut.push_back(v[k].y);
		vOut.push_back(v[k].z);

		vOut.push_back(n[k].x);
		vOut.push_back(n[k].y);
		vOut.push_back(n[k].z);
	}

	return vOut;
}

Math::Vec3 Physic::ConvertMouseToWorld(Math::Vec2 mousePos, Math::Vec2 screenSize, Render::Camera cameraPos)
{
	if (mousePos.x < 0 || mousePos.y < 0 || screenSize.x < mousePos.x || screenSize.y < mousePos.y)
		return Math::Vec3(INFINITY);
	auto position = cameraPos.GetViewMatrix() * Math::Vec3(mousePos.x, mousePos.y, 0);
	return position;
}
