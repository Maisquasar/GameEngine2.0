#include "Include/Physic/BoxCollider.h"
#include "Include/Core/Transform.h"

using namespace Math;
Physic::BoxCollider::BoxCollider()
{
}

Physic::BoxCollider::BoxCollider(Math::Vector3 Position, Math::Vector3 Size)
{
}

const Math::Vector3& Physic::BoxCollider::GetMin() const
{
	return _minimum;
}

const Math::Vector3& Physic::BoxCollider::GetMax() const
{
	return _maximum;
}

void Physic::BoxCollider::Update()
{/*
	if (!Transform->IsDirty())
		return;
    // Find the local space coordinates of the corners
    std::array<Vector3, 8> localCorners = {
        Vector3(minimum.x, minimum.y, minimum.z),
        Vector3(minimum.x, minimum.y, maximum.z),
        Vector3(minimum.x, maximum.y, minimum.z),
        Vector3(minimum.x, maximum.y, maximum.z),
        Vector3(maximum.x, minimum.y, minimum.z),
        Vector3(maximum.x, minimum.y, maximum.z),
        Vector3(maximum.x, maximum.y, minimum.z),
        Vector3(maximum.x, maximum.y, maximum.z)
    };

    // Apply the box's orientation to each corner point
    std::array<Vector3, 8> worldCorners;
    for (size_t i = 0; i < 8; ++i)
    {
        worldCorners[i] = orientation * localCorners[i];
    }

    return worldCorners;*/
}


