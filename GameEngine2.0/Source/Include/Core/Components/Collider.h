#pragma once
#include <PhysX/PxPhysicsAPI.h>
namespace Core::Components {
	class Collider
	{
	public:
		Collider() {}
		~Collider();

		physx::PxRigidDynamic* GetRigidDynamic() { return _body; }

		virtual void InitializePhysics() {}

	protected:
		physx::PxRigidDynamic* _body = NULL;
	};
}