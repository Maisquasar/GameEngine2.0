#pragma once
#include "Component.h"
namespace Core::Components
{
	class Rigidbody : public BaseComponent<Rigidbody>
	{
	public:
		Rigidbody();
		~Rigidbody();

		void SetUIIcon();

		void ShowInInspector() override;

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;

		float GetMass() { return _mass; }
		Math::Vec3 GetInitialVelocity() { return _velocity; }
	private:
		float _mass = 10.f;
		Math::Vec3 _velocity = {0};

	};
}