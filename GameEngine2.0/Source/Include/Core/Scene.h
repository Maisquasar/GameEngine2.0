#pragma once
#include <string>
#include "Include/Render/Camera.h"
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

		// ======================== Getters ========================
		Node* GetSceneNode()					{ return _sceneNode; }
		Render::Camera* GetCameraEditor() { return &_cameraEditor; }
		Render::Gizmo* GetGizmo()				{ return &_gizmo; }
		std::string GetCurrentScenePath()		{ return _currentScenePath; }
		Math::Mat4 GetVPMatrix()				{ return _VP; }
		Math::Vec4 GetClearColor()			{ return _clearColor; }
	private:
		std::string _currentScenePath = "";
		Math::Mat4 _VP;
		Node* _sceneNode = nullptr;

		Math::Vec4 _clearColor = { 0.45f, 0.55f, 0.60f, 1.00f };

		// Render
		Render::Camera _cameraEditor;
		Render::Gizmo _gizmo;
		Render::EditorGrid _grid;
	};
}