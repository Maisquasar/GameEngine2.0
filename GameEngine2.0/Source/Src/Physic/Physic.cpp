#include "Include/Physic/Physic.h"
#include "Include/Physic/Ray.h"
#include "Include/Physic/BoxCollider.h"
/*
bool Physic::RayBoxIntersection(const Ray& ray, const Box& box)
{
    
    // Transform the ray into the box's local space
    Math::Vector3 localOrigin = ray.origin - box.position;
    Math::Vector3 localDirection = ray.direction;

    // Check for intersection with each of the box's sides
    float tmin = (box.min.x - localOrigin.x) / localDirection.x;
    float tmax = (box.max.x - localOrigin.x) / localDirection.x;

    if (tmin > tmax)
    {
        std::swap(tmin, tmax);
    }

    float tymin = (box.min.y - localOrigin.y) / localDirection.y;
    float tymax = (box.max.y - localOrigin.y) / localDirection.y;

    if (tymin > tymax)
    {
        std::swap(tymin, tymax);
    }

    if ((tmin > tymax) || (tymin > tmax))
    {
        return false;
    }

    if (tymin > tmin)
    {
        tmin = tymin;
    }

    if (tymax < tmax)
    {
        tmax = tymax;
    }

    float tzmin = (box.min.z - localOrigin.z) / localDirection.z;
    float tzmax = (box.max.z - localOrigin.z) / localDirection.z;

    if (tzmin > tzmax)
    {
        std::swap(tzmin, tzmax);
    }

    if ((tmin > tzmax) || (tzmin > tmax))
    {
        return false;
    }

    return true;
}
*/