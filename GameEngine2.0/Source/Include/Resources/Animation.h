#pragma once
#include "IResource.h"
#include <vector>
#include <unordered_map>
#include <map>
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

		float FrameRate = 0;

		size_t KeyCount = 0;

		// List of all Bones, With all Key Inside with :
		// - first = Current Keyframe
		// - second = Transform at this keyFrame
		std::vector < std::unordered_map<int, Math::Vector3>> KeyPositions;
		std::vector < std::unordered_map<int, Math::Quaternion>> KeyRotations;
	private:
	};
}