#pragma once
#include <Physx/PxPhysicsAPI.h>

#include "Component.h"
namespace Core::Components
{
	class Rigidbody : public BaseComponent<Rigidbody>
	{
	public:
		Rigidbody();
		~Rigidbody();

		void SetUIIcon();

		void BeginPause();

		void ShowInInspector() override;

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;

		void SetParameters(physx::PxRigidDynamic* body);

		float GetMass() { return _mass; }
		Math::Vec3 GetInitialVelocity() { return _velocity; }

	private:
		physx::PxRigidDynamic* _body;
		float _mass = 10.f;
		Math::Vec3 _velocity = { 0 };
		Math::Vec3 _angularVelocity = { 0 };
		bool _useGravity = true;

	};
}