#pragma once
#include "Include/Math/Math.h"
#include <vector>

namespace Core::Components
{
    class BoxCollider;
}
namespace Render
{
    class Camera;
}

namespace Physic
{
    class Ray;
    std::vector<float> GetCubeVertices();
    std::vector<float> GetSphereVertices(float radius);
    std::vector<float> GetDemiSphereVertices(float radius, bool up);
    std::vector<float> GetCylindreVertices(float radius, float height);
    Math::Vec3 ConvertMouseToWorld(Math::Vec2 mousePos, Math::Vec2 screenSize, Render::Camera cameraPos);
}