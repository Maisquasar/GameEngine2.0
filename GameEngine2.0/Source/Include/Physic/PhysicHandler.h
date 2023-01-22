#pragma once
#include <vector>
#include <PhysX/PxPhysicsAPI.h>

#include "Include/Math/Math.h"
namespace Core::Components
{
	class Collider;
}

namespace Physic {
	struct PhysicMaterial
	{
		float static_friction = 0.5f;
		float dynamic_friction = 0.5f;
		float restitution = 0.6f;
	};

	class PhysicHandler
	{
	public:
		PhysicHandler();
		~PhysicHandler();

		void BeginPlay();
		void Initialize();
		void Update();

		void AddCollider(Core::Components::Collider* object);
		void RemoveCollider(Core::Components::Collider* object);
		physx::PxRigidDynamic* CreateCube(const Math::Vec3& shape, physx::PxTransform transform, float mass = 10.f);

	private:
		physx::PxDefaultAllocator      _defaultAllocatorCallback;
		physx::PxDefaultErrorCallback  _defaultErrorCallback;
		physx::PxDefaultCpuDispatcher* _dispatcher = NULL;
		physx::PxTolerancesScale _toleranceScale;
		physx::PxPvd* _pvd = NULL;
		physx::PxPvdTransport* _transport = NULL;
		physx::PxPvdSceneClient* _sceneClient = NULL;
		physx::PxFoundation* _foundation = NULL;

		physx::PxScene* _scene = NULL;
		physx::PxPhysics* _physics = NULL;
		physx::PxMaterial* _defaultMaterial = NULL;
		std::vector<Core::Components::Collider*> _objects;
	};
}