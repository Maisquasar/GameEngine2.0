#pragma once
#include "Component.h"
#include "Include/Render/Camera.h"
namespace Render
{
	class EditorIcon;
	class InstancesManager;
}

namespace Core::Components
{

	class CameraComponent : public BaseComponent<CameraComponent>, public Render::Camera
	{
	public:
		CameraComponent();
		~CameraComponent();

		void Initialize() override;
		void Update() override;
		void DrawPicking(int id) override;

		void SetUIIcon() override;

		Render::EditorIcon* Icon = nullptr;
	private:
	};
}
