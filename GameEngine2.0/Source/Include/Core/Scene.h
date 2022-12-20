#pragma once
#include <string>
#include "Include/Render/CameraEditor.h"
#include "Include/Render/Framebuffer.h"
#include "Include/Render/Gizmo.h"
#include "Include/Render/EditorGrid.h"


namespace Core
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void Initialize();
		void Update();
		void PickingUpdate(std::vector<Node*>);

		// Save functions
		void SaveScene();
		void SaveNode(std::string Path, Core::Node* node);

		// Load functions
		void LoadScene(std::string Path);
		Core::Node* LoadNode(std::string Path);
		void LoadTemporaryScene(std::string Path);

		// Getters
		Node* GetSceneNode() { return _sceneNode; }
		Render::CameraEditor* GetCameraEditor() { return &_cameraEditor; }

	private:
		std::string _currentScenePath = "";
		Math::Matrix4 _VP;
		Node* _sceneNode = nullptr;

		Math::Vector4 _clearColor = { 0.45f, 0.55f, 0.60f, 1.00f };

		// Render
		Render::FrameBuffer* _frameBuffer = nullptr;
		Render::CameraEditor _cameraEditor;
		Render::Gizmo _gizmo;
		Render::EditorGrid _grid;
	};
}