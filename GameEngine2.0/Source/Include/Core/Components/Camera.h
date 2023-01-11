#pragma once
#include "Component.h"
#include "Include/Render/CameraEditor.h"
namespace Render
{
	class EditorIcon;
	class InstancesManager;
}

namespace Core::Components
{

	class Camera : public BaseComponent<Camera>, public Render::CameraEditor
	{
	public:
		Camera();
		~Camera();

		void Initialize() override;
		void Update() override;
		void DrawPicking(int id) override;

		void SetUIIcon() override;

		Render::EditorIcon* Icon = nullptr;
	private:
	};
}
