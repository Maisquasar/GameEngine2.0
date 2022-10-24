#pragma once
#include <ImGui/imgui.h>
#include "Include/Math/Math.h"
#include "Include/Utils/Input.h"

namespace Render {
	class CameraEditor
	{
	public:
		CameraEditor();
		~CameraEditor();

		Math::Vector3 Position;
		Math::Vector3 FocusPosition;
		Math::Vector2 Rotation = Math::Vector2(-PI / 2, -PI / 2);
		Math::Vector3 UpVector = Math::Vector3(0, 1, 0);
		float Distance = 5.f;
		float FOV = 70;
		float Near = 0.1f;
		float Far = 1000.f;
		float AspectRatio = (4.0f / 3.0f);
		float Speed = 25.f;

		void Update();
		// Return the Mat4D view matrix from the camera.
		Math::Matrix4 GetViewMatrix();
		// Return the Mat4D projection matrix from the camera.
		Math::Matrix4 GetProjection();
		// Return the Mat4D model matrix.
		Math::Matrix4 GetModelMatrix();
	private:

	};
}