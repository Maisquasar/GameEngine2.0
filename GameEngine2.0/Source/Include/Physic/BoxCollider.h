#pragma once
#include "Include/Math/Math.h"

namespace Core
{
	class Transform;
}

namespace Physic {
	class BoxCollider {
	public:
		// Constructors
		BoxCollider();
		BoxCollider(Math::Vector3 Position, Math::Vector3 Size);

		// Getters and setters
		const Math::Vector3& GetMin() const;
		const Math::Vector3& GetMax() const;

		void Update();

	private:
		Math::Vector3 _minimum;
		Math::Vector3 _maximum;
	};
}