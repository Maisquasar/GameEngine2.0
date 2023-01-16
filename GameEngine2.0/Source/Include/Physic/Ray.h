#pragma once
#include "Include/Math/Math.h"

namespace Physic
{
    class Ray
    {
    public:
        Ray() {}

        // Set the origin and direction of the ray
        void Set(const Math::Vec3& origin, const Math::Vec3& direction)
        {
            _origin = origin;
            _direction = direction.GetNormal();
        }

        // Return the origin of the ray
        Math::Vec3 GetOrigin() const { return _origin; }

        // Return the direction of the ray
        Math::Vec3 GetDirection() const { return _direction; }

        // Return the point on the ray at the given distance
        Math::Vec3 GetPoint(float distance) const { return _origin + _direction * distance; }

    private:
        Math::Vec3 _origin;    // The origin of the ray
        Math::Vec3 _direction; // The direction of the ray
    };
}