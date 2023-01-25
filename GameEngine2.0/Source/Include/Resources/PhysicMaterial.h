#pragma once
#include "IResource.h"
#include <PhysX/PxPhysicsAPI.h>
namespace Resources {
	class PhysicMaterial : public BaseResource<PhysicMaterial>
	{
	public:
		PhysicMaterial();
		~PhysicMaterial();

		void Load(std::string filename) override;
		void MultiThreadLoad(std::string filename);

		void Save();

		void Initialize();

		void ShowInInspector();

		physx::PxMaterial* const* GetMaterials() { return &_physxMaterial; }
		physx::PxMaterial* GetMaterial() { return _physxMaterial; }

	private:
		float _staticFriction = 0.5f;
		float _dynamicFriction = 0.5f;
		float _restitution = 0.5f;
		physx::PxMaterial* _physxMaterial = NULL;
	};
}


