#pragma once
#include "Component.h"
#include "Collider.h"
namespace Resources {
	class Shader;
}

namespace Core::Components
{
	class CapsuleCollider : public BaseComponent<CapsuleCollider>, public Collider
	{
	public:
		CapsuleCollider();
		~CapsuleCollider();

		void Initialize() override;
		void InitializePhysics() override;

		void GameUpdate() override;
		void Draw() override;
		void DrawPicking(int id) override;

		void ShowInInspector() override;
		void SetUIIcon() override;

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;
	private:
		physx::PxRigidDynamic* _dynamicBody;
		physx::PxRigidStatic* _staticBody;
		unsigned int _VBO[3] = { 0, 0, 0 };
		unsigned int _VAO[3] = { 0, 0, 0 };
		std::vector<float> _vertices[3];
		Resources::Shader* _shader;

		Math::Vec3 _center = Math::Vec3(0);
		float _height = 1.f;
		float _radius = 1.f;
	};
}