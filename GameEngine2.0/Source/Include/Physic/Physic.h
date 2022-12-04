#pragma once
#include "Include/Math/Math.h"

namespace Physic
{
    class Ray;
    class Box;
    bool RayBoxIntersection(const Ray& ray, const Box& box);
}