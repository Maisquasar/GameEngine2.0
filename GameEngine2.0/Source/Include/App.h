#pragma once
#include "Include/Debug/Log.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <filesystem>

#include "Include/Core/Node.h"
#include "Include/EditorUi/Editor.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/Resources/Model.h"
#include "Include/Render/CameraEditor.h"
#include "Include/Render/Framebuffer.h"
#include "Include/Render/Gizmo.h"
#include "Include/Utils/Input.h"
#include "Include/Utils/ThreadManager.h"
#include "Include/Utils/AppSettings.h"
#include "Include/Core/Scene.h"

enum class GameState
{
	Play,
	Editor,
};

class App
{
public:
	App();
	App(const char* Name, int width, int height);
	~App();

	void InitializeApp(const char* Name, int width, int height);
	void Update();
	void ClearApp();
	void CloseApp();

	void LoadScene(std::string Path);
	Core::Node* LoadNode(std::string Path);
	void LoadTemporaryScene(std::string Path);
	void SaveScene();
	void SaveNode(std::string Path, Core::Node* node);

	void BeginPlay();
	void EndPlay();

	GLFWwindow* GetWindow() { return _window; }

	Math::Integer2 GetWindowSize();

	const GLFWvidmode* GetMonitorVideoMode();

	Math::Matrix4 GetVPMatrix() { return _VP; }

	GameState GetGameState() { return _gameState; }

	std::string GetCurrentScenePath() { return _currentScenePath; }

	float GetDeltaTime() { return (float)_deltaTime; }

	Render::CameraEditor* GetEditorCamera() { return &_cameraEditor; }

	Render::Gizmo* GetGizmo() { return &_gizmo; }

	Render::FrameBuffer* GetFramebuffer() { return &_framebuffer; }

	Utils::AppSettings* GetSettings() { return &_settings; }

	Resources::ResourceManager* GetResourceManager() { return &_resourceManager; }

	Core::Scene* GetScene() { return &_scene; }

	Core::Node* SceneNode = nullptr;
	Core::Components::Data Components;
	Utils::ThreadManager ThreadManager;
	std::vector<Resources::IResource**> MultiThreadMeshes;
private:
	// Window
	GLFWwindow* _window = nullptr;
	const GLFWvidmode* _videoMode = nullptr;
	bool _shouldClose = false;
	Math::Matrix4 _VP = Math::Matrix4::Identity();
	GameState _gameState = GameState::Editor;
	std::string _currentScenePath = "";
	double currentFrame = 0;
	double _lastFrame = 0;
	double _deltaTime = 0;

	const char* _windowName = "GameEngine2.0";
	bool _everythingIsLoaded = false;
	int _width = 800;
	int _height = 600;

	Math::Vector4 _clearColor = {0.45f, 0.55f, 0.60f, 1.00f};
	
	// Ui
	EditorUi::Editor _editorUi;

	// Managers
	Resources::ResourceManager _resourceManager;

	// Utils
	Utils::Input _input;
	Utils::AppSettings _settings;

	// Render
	Render::CameraEditor _cameraEditor;
	Render::FrameBuffer _framebuffer;
	Render::Gizmo _gizmo;

	Core::Scene _scene;

	void InitGlfw();
	void InitImGui();
	void InitGlad();
	void LoadResources();

	void FilesLoad(std::string path);

	void MultiThreadLoad();
	void PickingUpdate(std::vector<Core::Node*> nodes);
};

extern App Application;

//TODO: Lights