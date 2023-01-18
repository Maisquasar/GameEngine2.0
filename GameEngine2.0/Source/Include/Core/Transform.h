#pragma once
#include <stdio.h>
#include <vector>
#include <filesystem>
#include "Include/Math/Math.h"
#include "Include/Core/Components/Component.h"

namespace Core
{
	class Node;
	class Transform : public Components::BaseComponent<Transform>
	{
	public:
		Transform();
		~Transform();

		// Nodes
		Node* Parent = nullptr;

		void ComputeModelMatrix();
		void ComputeModelMatrix(const Math::Mat4&);

		// World Setters
		void SetWorldPosition(Math::Vec3);
		void SetWorldRotation(Math::Quat);
		void SetWorldScale(Math::Vec3);

		// World Getters
		Math::Vec3 GetWorldPosition();
		Math::Quat GetWorldRotation();
		Math::Vec3 GetWorldScale();
		Math::Mat4 GetModelMatrix(bool update = false);

		// Local Setters
		void SetLocalPosition(Math::Vec3);
		void SetLocalRotation(Math::Quat);
		void SetLocalScale(Math::Vec3);

		// Local Getters
		Math::Vec3 GetLocalPosition();
		Math::Quat GetLocalRotation();
		Math::Vec3 GetLocalScale();
		Math::Mat4 GetLocalModelMatrix();

		Math::Vec3 GetForwardVector();
		Math::Vec3 GetRightVector();
		Math::Vec3 GetUpVector();

		Math::Vec3 TranformPoint(Math::Vec3);

		void RotateAround(Math::Vec3 point, Math::Vec3 axis, float angle);

		void Update();
		void ForceUpdate();

		void ShowInInspector();

		void Save(std::string space, std::string& content) override;

		bool IsDirty() { return _dirty; }

		void SetDirty() { _dirty = true; }

	private:
		Math::Mat4 _modelMatrix = Math::Mat4::Identity();
		Math::Vec3 _localPosition = Math::Vec3();
		Math::Quat _localRotation = Math::Quat();
		Math::Vec3 _localEulerRotation = Math::Vec3();
		Math::Vec3 _localScale = Math::Vec3(1);
		bool _dirty = true;

	};
}