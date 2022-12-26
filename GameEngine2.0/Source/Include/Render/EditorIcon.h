#pragma once
#include "Include/Math/Math.h"

namespace Resources{
	class Mesh;
}
namespace Core
{
	class Transform;
}
namespace Render {

	// TO USE IT :
	// Add Initialize to be executed one time.
	// Call Draw on Update of the Component.
	// Call Draw Picking on Drawpicking of the component.
	class EditorIcon
	{
	public:
		EditorIcon();
		~EditorIcon();

		void Initialize(std::string TextName);
		void Draw(Math::Matrix4 VP, Core::Transform transform);
		void DrawPicking(Math::Matrix4 MVP, Core::Transform transform, int id);

		Resources::Mesh* Plane = nullptr;

		Math::Matrix4 GetMVP(Math::Matrix4 VP, Core::Transform transform);

		void SetIconSize(float size) { IconSize = size; }
	private:
		float IconSize = 0.5f;

	};
}