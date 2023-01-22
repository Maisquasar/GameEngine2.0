#pragma once
#include <PhysX/PxPhysicsAPI.h>
namespace Core::Components {
	class Collider
	{
	public:
		Collider() {}
		~Collider() {}

		virtual void InitializePhysics() {}
	protected:
		physx::PxRigidDynamic* _body = NULL;
	private:

	};
}