#pragma once
#include "Include/Math/Math.h"

namespace Resources{
	class BillBoard;
	class Material;
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
		void Draw(Math::Mat4 VP, Core::Transform transform, bool selected = false);
		void DrawPicking(Math::Mat4 MVP, Core::Transform transform, int id);
		void SetSize(Math::Vec2 size);

		Resources::BillBoard* Plane = nullptr;

		Math::Mat4 GetMVP(const Math::Mat4& VP, Core::Transform& transform);

		void SetIconSize(float size) { IconSize = size; }
	private:
		float IconSize = 0.5f;
		Resources::Material* _material = nullptr;

	};
}