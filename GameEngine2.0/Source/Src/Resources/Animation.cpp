#include "Include/Resources/Animation.h"

#include <algorithm>

Resources::Animation::Animation() {}

Resources::Animation::~Animation() {}


void Resources::Animation::Initialize()
{
	auto max_size_position = [&](const std::vector < std::unordered_map<int, Math::Vector3>>& a) -> int
	{
		int max = 0;

		for (int x = 0; x < a.size(); x++)
		{
			for (auto i : a[x])
			{
				max = std::max(i.first, max);
			}
		}
		return max;
	};

	auto max_size_rotation = [&](const std::vector < std::unordered_map<int, Math::Quaternion>>& a) -> int
	{
		int max = 0;

		for (int x = 0; x < a.size(); x++)
		{
			for (auto i : a[x])
			{
				max = std::max(i.first, max);
			}
		}
		return max;
	};

	auto nearest_values_pos = [&](const std::unordered_map<int, Math::Vector3>& m, int target) -> std::pair<int, int>
	{
		int lower = -1, upper = -1;
		for (auto i : m) {
			if (i.first < target) {
				lower = i.first;
			}
			else if (i.first > target) {
				upper = i.first;
				break;
			}
		}
		return std::make_pair(lower, upper);
	};

	auto nearest_values_rot = [&](const std::unordered_map<int, Math::Quaternion>& m, int target) -> std::pair<int, int>
	{
		int lower = -1, upper = -1;
		for (auto i : m) {
			if (i.first < target) {
				lower = i.first;
			}
			else if (i.first > target) {
				upper = i.first;
				break;
			}
		}
		return std::make_pair(lower, upper);
	};

	KeyCount = std::max(max_size_rotation(KeyRotations), max_size_position(KeyPositions));
	for (int i = 0; i < KeyCount; i++)
	{
		for (auto& Key : KeyPositions)
		{
			if (Key.find(i) == Key.end())
			{
				auto nearests = nearest_values_pos(Key, i);
				int lower = nearests.first;
				int upper = nearests.second;
				Key[i] = Math::Vector3::Lerp(Key[lower], Key[upper], (float)i / (float)KeyCount);
			}

		}
		for (auto& Key : KeyRotations)
		{
			if (Key.find(i) == Key.end())
			{
				auto nearests = nearest_values_rot(Key, i);
				int lower = nearests.first;
				int upper = nearests.second;
				Key[i] = Math::Quaternion::SLerp(Key[lower], Key[upper], (float)i / (float)KeyCount);
			}

		}
	}
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
			Position = Math::Vector3::Lerp(PreviousFramePos, NextFramePos, (float)(time - pt0) / (float)(pt1 - pt0));
		}
		else if (KeyPositions.size() > id && KeyPositions[id].size() > 0)
		{
			Position = KeyPositions[id].begin()->second;
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
			Rotation = KeyRotations[id].begin()->second;
		}
	}
}
