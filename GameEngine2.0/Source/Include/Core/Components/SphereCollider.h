#pragma once
#include "Component.h"
#include "Collider.h"
#include "Include/Core/Transform.h"
namespace Resources{
	class Shader;
}
namespace Core
{
	class Node;
	namespace Components {
		class SphereCollider : public BaseComponent<SphereCollider>, public Collider
		{
		public:
			SphereCollider();
			~SphereCollider();

			void Initialize() override;
			void InitializePhysics() override;

			void SetGameobject(Core::Node* node) override;
			void GameUpdate() override;
			void Draw() override; 
			void UpdateTransform() override;

			void ShowInInspector() override;
			void SetUIIcon() override;

			Math::Mat4 GetModelMatrix();
			//TODO : Add Save/Load
			void Save(std::string space, std::string& content);
			void Load(const char* data, uint32_t& pos);
		private:
			physx::PxRigidDynamic* _dynamicBody;
			physx::PxRigidStatic* _staticBody;
			unsigned int _VBO = 0;
			unsigned int _VAO = 0;
			std::vector<float> _vertices;
			Resources::Shader* _shader;
			float _radius = 1.f;

		};
	}
}