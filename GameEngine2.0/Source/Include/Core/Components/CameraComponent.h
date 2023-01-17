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
		void EditorUpdate() override;
		void EditorDraw() override;
		void DrawPicking(int id) override;

		void ShowInInspector() override;
		
		void SetMainCamera();

		void SetUIIcon() override;
		bool IsVisible() override;

		class Transform* GetTransform() override;

		Render::EditorIcon* Icon = nullptr;

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;
	private:
		bool _MainCamera = false;
	};
}
