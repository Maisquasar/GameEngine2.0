#pragma once
#include <PhysX/PxPhysicsAPI.h>
#include <Include/Math/Math.h>

namespace Core::Components {

	bool CheckDif(const physx::PxTransform& transform, Math::Mat4 mat);
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