#pragma once
#include "Include/Resources/Mesh.h"
#include "Include/Core/Transform.h"

namespace Core
{
	class Node;
}

namespace Render
{
	class Gizmo
	{
	public:
		Gizmo();
		~Gizmo();

		void Draw();
		void DrawPicking(int id);

		Core::Transform* NodeTransform = nullptr;
		Core::Node** GetNodes() { return _axis; }
		Math::Matrix4 GetMVP();

		float ForwardDistance = 0;
	private:
		Core::Node* _axis[4];
		bool _foundMesh[4] = {false};
		bool _initialized = false;
	};
}