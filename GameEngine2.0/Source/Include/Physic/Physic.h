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
    bool RayBoxIntersection(Ray* ray, Core::Components::BoxCollider* box, Math::Vector3& intersectionPoint);
    std::vector<float> GetCubeVertices();
    Math::Vector3 ConvertMouseToWorld(Math::Vector2 mousePos, Math::Vector2 screenSize, Render::CameraEditor cameraPos);
}