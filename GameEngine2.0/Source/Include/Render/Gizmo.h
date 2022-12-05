#pragma once
#include "Include/Resources/Mesh.h"
#include "Include/Core/Transform.h"

namespace Render
{
	class Gizmo
	{
	public:
		Gizmo();
		~Gizmo();

		void Draw();
		Core::Transform* NodeTransform;
	private:
		Resources::Mesh* _mesh;
	};
}