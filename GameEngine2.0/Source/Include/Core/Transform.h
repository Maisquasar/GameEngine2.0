#pragma once
#include <stdio.h>
#include <vector>
#include <filesystem>
#include "Include/Math/Math.h"

namespace Core
{
	class Node;
	class Transform
	{
	public:
		Transform();
		~Transform();

		// Nodes
		Node* Parent = nullptr;
		Node* GameObject = nullptr;

		// Local Setters
		void SetLocalPosition(Math::Vector3);
		void SetLocalRotation(Math::Quaternion);
		void SetLocalScale(Math::Vector3);

		// Local Getters
		Math::Vector3 GetLocalPosition();
		Math::Quaternion GetLocalRotation();
		Math::Vector3 GetLocalScale();

	private:
		Math::Matrix4 _modelMatrix = Math::Matrix4::Identity();
		Math::Vector3 _localPosition = Math::Vector3();
		Math::Quaternion _localRotation = Math::Quaternion();
		Math::Vector3 _localScale = Math::Vector3(1);
		bool _dirty = true;

	};
}