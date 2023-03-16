#pragma once
#include "Include/Math/Math.h"

namespace Resources{
	class Material;
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
		void Draw(Math::Mat4 VP, Core::Transform transform, bool selected = false);
		void DrawPicking(Math::Mat4 MVP, Core::Transform transform, int id);
		void SetSize(Math::Vec2 size);

		Resources::Mesh* Plane = nullptr;

		Math::Mat4 GetMVP(const Math::Mat4& VP, Core::Transform& transform);

		void SetIconSize(Math::Vec2 size) { IconSize = size; }
	private:
		Math::Vec2 IconSize = Math::Vec2(0.5f , 0.5f);
		Resources::Material* _material = nullptr;

	};
}