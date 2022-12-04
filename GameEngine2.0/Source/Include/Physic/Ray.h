#pragma once
#include "Include/Math/Math.h"

namespace Physic
{
    class Ray
    {
    public:
        Ray() {}

        // Set the origin and direction of the ray
        void Set(const Math::Vector3& origin, const Math::Vector3& direction)
        {
            _origin = origin;
            _direction = direction.Normalize();
        }

        // Return the origin of the ray
        Math::Vector3 GetOrigin() const { return _origin; }

        // Return the direction of the ray
        Math::Vector3 GetDirection() const { return _direction; }

        // Return the point on the ray at the given distance
        Math::Vector3 GetPoint(float distance) const { return _origin + _direction * distance; }

    private:
        Math::Vector3 _origin;    // The origin of the ray
        Math::Vector3 _direction; // The direction of the ray
    };
}