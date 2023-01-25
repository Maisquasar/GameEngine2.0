#pragma once
#include <PhysX/PxPhysicsAPI.h>
#include <Include/Math/Math.h>
namespace Resources
{
	class PhysicMaterial;
}
namespace Core::Components {

	bool CheckDif(const physx::PxTransform& transform, Math::Mat4 mat);
	class Collider
	{
	public:
		Collider();
		~Collider();

		virtual void InitializePhysics() {}

	protected:
		physx::PxRigidDynamic* _dynamicBody;
		physx::PxRigidStatic* _staticBody;

		physx::PxShape* _shape = NULL;

		Resources::PhysicMaterial* _physicalMaterial;
	};
}