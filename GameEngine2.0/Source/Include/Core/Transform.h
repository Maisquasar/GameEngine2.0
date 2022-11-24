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

		void ComputeModelMatrix();
		void ComputeModelMatrix(Math::Matrix4);

		// World Setters
		void SetWorldPosition(Math::Vector3);
		void SetWorldRotation(Math::Quaternion);
		void SetWorldScale(Math::Vector3);

		// World Getters
		Math::Vector3 GetWorldPosition();
		Math::Quaternion GetWorldRotation();
		Math::Vector3 GetWorldScale();
		Math::Matrix4 GetModelMatrix();

		// Local Setters
		void SetLocalPosition(Math::Vector3);
		void SetLocalRotation(Math::Quaternion);
		void SetLocalScale(Math::Vector3);

		// Local Getters
		Math::Vector3 GetLocalPosition();
		Math::Quaternion GetLocalRotation();
		Math::Vector3 GetLocalScale();
		Math::Matrix4 GetLocalModelMatrix();

		Math::Vector3 GetForwardVector();
		Math::Vector3 GetRightVector();
		Math::Vector3 GetUpVector();

		void RotateAround(Math::Vector3 point, Math::Vector3 axis, float angle);

		void Update();
		void ForceUpdate();

		void ShowInInspector();

	private:
		Math::Matrix4 _modelMatrix = Math::Matrix4::Identity();
		Math::Vector3 _localPosition = Math::Vector3();
		Math::Quaternion _localRotation = Math::Quaternion();
		Math::Vector3 _localScale = Math::Vector3(1);
		bool _dirty = true;

	};
}