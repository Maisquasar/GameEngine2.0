#pragma once
#include "Include/Resources/Mesh.h"
#include "Include/Core/Transform.h"

namespace Core
{
	class Node;
}

namespace Render
{
	enum class GizmoMode : int
	{
		Translate,
		Rotate,
		Scale
	};

	class Gizmo
	{
	public:
		Gizmo();
		~Gizmo();

		void Draw();
		void DrawPicking(int id);
		void Update(size_t ArrowClicked, Math::Vector2& mousePosition);

		Core::Transform* NodeTransform = nullptr;
		Core::Node** GetNodes() { return _axis; }
		Math::Matrix4 GetMVP();

		float ForwardDistance = 0;
		GizmoMode Mode = GizmoMode::Translate;
	private:
		Core::Node* _axis[4];
		bool _foundMesh[4] = { false };
		bool _initialized = false;
	};
}