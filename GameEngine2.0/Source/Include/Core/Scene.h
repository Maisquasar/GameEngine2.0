#pragma once
#include <string>
#include "Include/Render/Camera.h"
#include "Include/Render/Gizmo.h"
#include "Include/Render/EditorGrid.h"

namespace Render
{
	class Camera;
}
namespace EditorUi {
	class SceneWindow;
}
namespace Physic
{
	class PhysicHandler;
}
namespace Core
{
	namespace Components
	{
		class CameraComponent;
	}
	class Scene
	{
	public:
		Scene();
		~Scene();

		void Initialize();
		// Call Before Start
		void BeginPlay();
		void Update();
		void RenderScene();
		void PickingUpdate(std::vector<Node*>);

		// Save functions
		void SaveScene();
		void SaveNode(std::string Path, Core::Node* node);

		// Load functions
		void LoadScene(std::string Path, bool temporary = false);
		Core::Node* LoadNode(std::string Path);
		void LoadTemporaryScene(std::string Path);

		void SetCurrentCamera(Render::Camera* camera);
		void SetMainCamera(Render::Camera* camera);

		// ======================== Getters ========================
		Node* GetSceneNode() { return _sceneNode; }
		Render::Camera* GetCameraEditor() { return &_cameraEditor; }
		Render::Camera* GetCurrentCamera() { return _currentCamera; }
		Render::Camera* GetMainCamera() { return _mainCamera; }
		Render::Gizmo* GetGizmo() { return &_gizmo; }
		std::string GetCurrentScenePath() { return _currentScenePath; }
		Math::Mat4 GetVPMatrix() { return _VP; }
		Math::Vec4 GetClearColor() { return _clearColor; }
		std::vector<Render::Camera*> Cameras;

	private:
		std::string _currentScenePath = "";
		bool _showGrid = true;

		Math::Mat4 _VP;

		Node* _sceneNode = nullptr;

		Render::Camera* _currentCamera;
		Render::Camera* _mainCamera;

		Math::Vec4 _clearColor = { 0.45f, 0.55f, 0.60f, 1.00f };

		// Render
		Render::Camera _cameraEditor;
		Render::Gizmo _gizmo;
		Render::EditorGrid _grid;

		friend class EditorUi::SceneWindow;
	};
}