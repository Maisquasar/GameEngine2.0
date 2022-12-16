#include "Include/Resources/Animation.h"

Resources::Animation::Animation() {}

Resources::Animation::~Animation() {}

void Resources::Animation::Initialize()
{
}

void Resources::Animation::GetAnimAtFrame(int id, float time, Math::Vector3& Position, Math::Quaternion& Rotation)
{
	int pt0, pt1, rt0, rt1;
	if (time >= 0) {
		// Position
		if (KeyPositions.size() > id && KeyPositions[id].size() > 1) {
			pt0 = (int)std::floor(abs(time)) % (KeyPositions[id].size() - 1);
			pt1 = (int)std::ceil(abs(time)) % (KeyPositions[id].size() - 1);
			if (pt1 == pt0)
				pt1 += 1;

			// Lerp The Position
			Math::Vector3 PreviousFramePos = KeyPositions[id][pt0];
			Math::Vector3 NextFramePos = KeyPositions[id][pt1];
			Position = Math::Vector3::SLerp(PreviousFramePos, NextFramePos, (float)(time - pt0) / (float)(pt1 - pt0));
		}
		else if (KeyPositions.size() > id && KeyPositions[id].size() > 0)
		{
			Position = KeyPositions[id].back();
		}

		// Rotation
		if (KeyRotations.size() > id && KeyRotations[id].size() > 1) {
			rt0 = (int)std::floor(abs(time)) % (KeyRotations[id].size() - 1);
			rt1 = (int)std::ceil(abs(time)) % (KeyRotations[id].size() - 1);
			if (rt1 == rt0)
				rt1 += 1;

			// Lerp The Rotation
			Math::Quaternion PreviousFrameRot = KeyRotations[id][rt0];
			Math::Quaternion NextFrameRot = KeyRotations[id][rt1];
			Rotation = Math::Quaternion::SLerp(PreviousFrameRot, NextFrameRot, (float)(time - rt0) / (float)(rt1 - rt0));
		}
		else if (KeyRotations.size() > id && KeyRotations[id].size() > 0)
		{
			Rotation = KeyRotations[id].back();
		}
	}
}
