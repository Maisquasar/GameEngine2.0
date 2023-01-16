#include "Include/Physic/Physic.h"
#include "Include/Physic/Ray.h"
#include "Include/Core/Transform.h"
#include "Include/Core/Components/BoxCollider.h"
#include "Include/Render/Camera.h"

bool Physic::RayBoxIntersection(Ray* ray, Core::Components::BoxCollider* box, Math::Vec3& intersectionPoint)
{
	// Transform the ray into the local space of the box.
	Math::Vec3 localRayOrigin = box->Transform.GetWorldRotation().GetInverse() * (ray->GetOrigin() - box->Transform.GetWorldPosition());
	Math::Vec3 localRayDirection = box->Transform.GetWorldRotation().GetInverse() * ray->GetDirection();

	// Compute the minimum and maximum t values for the ray.
	float tMin = -INFINITY;
	float tMax = INFINITY;

	// Check intersection with the x-aligned planes of the box.
	if (std::abs(localRayDirection.x) > 1e-6)
	{
		float t1 = (box->Transform.GetWorldScale().x - localRayOrigin.x) / localRayDirection.x;
		float t2 = (-box->Transform.GetWorldScale().x - localRayOrigin.x) / localRayDirection.x;
		tMin = max(tMin, min(t1, t2));
		tMax = min(tMax, max(t1, t2));
	}

	// Check intersection with the y-aligned planes of the box.
	if (std::abs(localRayDirection.y) > 1e-6)
	{
		float t1 = (box->Transform.GetWorldScale().y - localRayOrigin.y) / localRayDirection.y;
		float t2 = (-box->Transform.GetWorldScale().y - localRayOrigin.y) / localRayDirection.y;
		tMin = max(tMin, min(t1, t2));
		tMax = min(tMax, max(t1, t2));
	}

	// Check intersection with the z-aligned planes of the box.
	if (std::abs(localRayDirection.z) > 1e-6)
	{
		float t1 = (box->Transform.GetWorldScale().z - localRayOrigin.z) / localRayDirection.z;
		float t2 = (-box->Transform.GetWorldScale().z - localRayOrigin.z) / localRayDirection.z;
		tMin = max(tMin, min(t1, t2));
		tMax = min(tMax, max(t1, t2));
	}

	// Check if there is an intersection.
	if (tMin > tMax)
		return false;

	// Compute the intersection point.
	intersectionPoint = ray->GetOrigin() + ray->GetDirection() * tMin;

	return true;
}


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

Math::Vec3 Physic::ConvertMouseToWorld(Math::Vec2 mousePos, Math::Vec2 screenSize, Render::Camera cameraPos)
{
	if (mousePos.x < 0 || mousePos.y < 0 || screenSize.x < mousePos.x || screenSize.y < mousePos.y)
		return Math::Vec3(INFINITY);
	auto position = cameraPos.GetViewMatrix() * Math::Vec3(mousePos.x, mousePos.y, 0);
	return position;
}
