#pragma once
#include "Include/Math/Math.h"
#include <vector>

namespace Core::Components
{
    class BoxCollider;
}
namespace Render
{
    class CameraEditor;
}

namespace Physic
{
    class Ray;
    bool RayBoxIntersection(Ray* ray, Core::Components::BoxCollider* box, Math::Vec3& intersectionPoint);
    std::vector<float> GetCubeVertices();
    Math::Vec3 ConvertMouseToWorld(Math::Vec2 mousePos, Math::Vec2 screenSize, Render::CameraEditor cameraPos);
}