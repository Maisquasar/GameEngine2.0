#pragma once
#include "IResource.h"
#include <vector>
#include "Include/Math/Math.h"


namespace Resources
{
	class Animation : public BaseResource<Animation>
	{
	public:
		Animation();
		~Animation();

		void Initialize() override;

		void GetAnimAtFrame(int id, float time, Math::Vector3& Position, Math::Quaternion& Rotation);

		size_t KeyPosCount = 0;
		size_t KeyRotCount = 0;


		std::vector < std::vector<Math::Vector3>> KeyPositions;
		std::vector < std::vector<Math::Quaternion>> KeyRotations;
	private:
	};
}