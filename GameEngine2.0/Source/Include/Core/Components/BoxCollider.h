#pragma once
#include "Include/Core/Components/Component.h"
#include "Include/Core/Transform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace Resources
{
	class Shader;
}

namespace Core::Components {
	class BoxCollider : public Core::Components::BaseComponent<BoxCollider>
	{
	public:
		typedef BaseComponent<BoxCollider> Super;
		// Constructors
		BoxCollider();
		BoxCollider(Math::Vector3 Position, Math::Vector3 Size, Math::Quaternion Rotation = Math::Quaternion());

		void SetGameobject(Core::Node* node) override;

		void Initialize() override;
		void Update() override;
		void ShowInInspector() override;

		Core::Transform Transform;
	private:
		unsigned int _VBO = 0;
		unsigned int _VAO = 0;
		std::vector<float> _vertices;
		Resources::Shader* _shader;
	};
}