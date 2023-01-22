#pragma once
#include <Windows.h>
#include "Include/Core/Transform.h"
#include "Include/Core/Components/Collider.h"

namespace Resources
{
	class Shader;
}

namespace Physic
{
	class Ray;
}

namespace Core::Components {
	class BoxCollider : public Core::Components::BaseComponent<BoxCollider>, public Collider
	{
	public:
		typedef BaseComponent<BoxCollider> Super;
		// Constructors
		BoxCollider();
		BoxCollider(Math::Vec3 Position, Math::Vec3 Size, Math::Quat Rotation = Math::Quat());

		~BoxCollider();

		void SetGameobject(Core::Node* node) override;

		void Initialize() override;

		void InitializePhysics() override;

		void Draw() override;
		void Update() override;
		void GameUpdate() override;
		void UpdateTransform() override;

		void ShowInInspector() override;
		void SetUIIcon() override;

		Core::Transform Transform;

		//TODO : Add Save/Load
	private:
		physx::PxRigidDynamic* _dynamicBody;
		physx::PxRigidStatic* _staticBody;
		unsigned int _VBO = 0;
		unsigned int _VAO = 0;
		std::vector<float> _vertices;
		Resources::Shader* _shader;
	};
}