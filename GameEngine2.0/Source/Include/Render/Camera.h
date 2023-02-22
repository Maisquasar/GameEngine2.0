#pragma once
#include "Include/Math/Math.h"
#include "Include/Core/Transform.h"
#include "Framebuffer.h"

namespace Render {
	class Camera
	{
	public:
		Camera();
		~Camera();

		Math::Vec3 Position;
		Math::Vec3 FocusPosition;
		Math::Vec2 Rotation = Math::Vec2(-PI / 2, -PI / 2);
		Math::Vec3 UpVector = Math::Vec3(0, 1, 0);
		float Distance = 5.f;
		float FOV = 70;
		float Near = 0.1f;
		float Far = 1000.f;
		float AspectRatio = (4.0f / 3.0f);
		float Speed = 25.f;

		void Initialize();

		void PreUpdate();

		void Update(bool firstUpdate = false);
		// Return the Mat4D view matrix from the camera.
		Math::Mat4 GetViewMatrix();
		// Return the Mat4D projection matrix from the camera.
		Math::Mat4 GetProjection();
		// Return the Mat4D model matrix.
		Math::Mat4 GetModelMatrix();

		Math::Vec3 GetUp();

		Math::Vec3 GetRight();

		Math::Vec3 UnProject(Math::Vec2 point);

		Math::Mat4 GetVP() { return GetProjection() * GetViewMatrix(); }

		virtual Core::Transform* GetTransform() { return &Transform; }

		virtual bool IsVisible();

		Render::FrameBuffer FrameBuffer;
	private:
		Core::Transform Transform;

	};
}