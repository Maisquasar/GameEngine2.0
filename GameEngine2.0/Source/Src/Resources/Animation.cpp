#include "Include/Resources/Animation.h"

#include <algorithm>

Resources::Animation::Animation() {}

Resources::Animation::~Animation() {}


void Resources::Animation::Initialize()
{
	auto max_size_position = [&](const std::vector < std::unordered_map<int, Math::Vec3>>& a) -> int
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

	auto max_size_rotation = [&](const std::vector < std::unordered_map<int, Math::Quat>>& a) -> int
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

	auto nearest_values_pos = [&](const std::unordered_map<int, Math::Vec3>& m, int target) -> std::pair<int, int>
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

	auto nearest_values_rot = [&](const std::unordered_map<int, Math::Quat>& m, int target) -> std::pair<int, int>
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
	//TODO : Fix This Commented For loop.
	/*
	for (int i = 0; i < KeyCount; i++)
	{
		for (auto& Key : KeyPositions)
		{
			if (Key.find(i) == Key.end())
			{
				auto nearests = nearest_values_pos(Key, i);
				int lower = nearests.first;
				int upper = nearests.second;
				Key[i] = Math::Vec3::Lerp(Key[lower], Key[upper], (float)i / (float)KeyCount);
			}

		}
		for (auto& Key : KeyRotations)
		{
			if (Key.find(i) == Key.end())
			{
				auto nearests = nearest_values_rot(Key, i);
				int lower = nearests.first;
				int upper = nearests.second;
				Key[i] = Math::Quat::SLerp(Key[lower], Key[upper], (float)i / (float)KeyCount);
			}

		}
	}
	*/
	SetInitialized();
}

void Resources::Animation::GetAnimAtFrame(int id, float time, Math::Vec3& Position, Math::Quat& Rotation)
{
	int pt0, pt1, rt0, rt1;
	if (time >= 0) {
		// Position
		if (KeyPositions.size() > id && KeyPositions[id].size() > 1) {
			pt0 = (int)std::floorf(abs(time)) % (KeyPositions[id].size() - 1);
			pt1 = (int)std::ceilf(abs(time)) % (KeyPositions[id].size() - 1);
			if (pt1 == pt0)
				pt1 += 1;

			// Lerp The Position
			Math::Vec3 PreviousFramePos = KeyPositions[id][pt0];
			Math::Vec3 NextFramePos = KeyPositions[id][pt1];
			Position = Math::Vec3::Lerp(PreviousFramePos, NextFramePos, (float)(time - pt0) / (float)(pt1 - pt0));
		}
		else if (KeyPositions.size() > id && KeyPositions[id].size() > 0)
		{
			Position = KeyPositions[id].begin()->second;
		}

		// Rotation
		if (KeyRotations.size() > id && KeyRotations[id].size() > 1) {
			rt0 = (int)std::floorf(abs(time)) % (KeyRotations[id].size() - 1);
			rt1 = (int)std::ceilf(abs(time)) % (KeyRotations[id].size() - 1);
			if (rt1 == rt0)
				rt1 += 1;

			// Lerp The Rotation
			Math::Quat PreviousFrameRot = KeyRotations[id][rt0];
			Math::Quat NextFrameRot = KeyRotations[id][rt1];
			Rotation = Math::Quat::SLerp(PreviousFrameRot, NextFrameRot, (float)(time - rt0) / (float)(rt1 - rt0));
		}
		else if (KeyRotations.size() > id && KeyRotations[id].size() > 0)
		{
			Rotation = KeyRotations[id].begin()->second;
		}
	}
}
