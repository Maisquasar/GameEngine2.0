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

		~BoxCollider();

		void SetGameobject(Core::Node* node) override;

		void Initialize() override;

		void InitializePhysics() override;

		void EndPause() override;

		void Draw() override;
		void DrawPicking(int id) override;
		void Update() override;
		void GameUpdate() override;

		void ShowInInspector() override;
		void SetUIIcon() override;

		void Save(std::string space, std::string& content);
		void Load(const char* data, uint32_t& pos);

	private:
		physx::PxRigidDynamic* _dynamicBody;
		physx::PxRigidStatic* _staticBody;
		Math::Vec3 _extent = Math::Vec3(1);
		unsigned int _VBO = 0;
		unsigned int _VAO = 0;
		std::vector<float> _vertices;
		Resources::Shader* _shader;
	};
}