#pragma once
#include <string>
#include "Include/Render/Camera.h"
#include "Include/Render/Framebuffer.h"
#include "Include/Render/Gizmo.h"
#include "Include/Render/EditorGrid.h"

namespace Render
{
	class Camera;
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
		void Update();
		void RenderScene();
		void PickingUpdate(std::vector<Node*>);

		// Save functions
		void SaveScene();
		void SaveNode(std::string Path, Core::Node* node);

		// Load functions
		void LoadScene(std::string Path);
		Core::Node* LoadNode(std::string Path);
		void LoadTemporaryScene(std::string Path);

		void SetCurrentCamera(Render::Camera* camera);

		// ======================== Getters ========================
		Node* GetSceneNode() { return _sceneNode; }
		Render::Camera* GetCameraEditor() { return &_cameraEditor; }
		Render::Camera* GetCurrentCamera() { return _currentCamera; }
		Render::Gizmo* GetGizmo() { return &_gizmo; }
		std::string GetCurrentScenePath() { return _currentScenePath; }
		Math::Mat4 GetVPMatrix() { return _VP; }
		Math::Vec4 GetClearColor() { return _clearColor; }
		std::vector<Render::Camera*> Cameras;
	private:
		std::string _currentScenePath = "";
		Math::Mat4 _VP;
		Node* _sceneNode = nullptr;
		Render::Camera* _currentCamera;

		Math::Vec4 _clearColor = { 0.45f, 0.55f, 0.60f, 1.00f };

		// Render
		Render::Camera _cameraEditor;
		Render::Gizmo _gizmo;
		Render::EditorGrid _grid;
	};
}