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

	void InitializeApp();
	void Update();
	void ClearApp();
	static void CloseApp();

	static void LoadScene(std::string Path);
	static Core::Node* LoadNode(std::string Path);
	static void LoadTemporaryScene(std::string Path);
	static void SaveScene();
	static void SaveNode(std::string Path, Core::Node* node);

	static void BeginPlay();
	static void EndPlay();

	static GLFWwindow* GetWindow() { return _window; }
	static Math::Integer2 GetWindowSize();
	static const GLFWvidmode* GetMonitorVideoMode();
	static Math::Matrix4 GetVPMatrix() { return _VP; }
	static GameState GetGameState() { return _gameState; }
	static std::string GetCurrentScenePath() { _currentScenePath; }
	static float GetDeltaTime() { return (float)_deltaTime; }
	static Render::CameraEditor* GetEditorCamera() { return &_cameraEditor; }
	static Render::Gizmo* GetGizmo() { return &_gizmo; }
	static Render::FrameBuffer* GetFramebuffer() { return &_framebuffer; }
	static Utils::AppSettings* GetSettings() { return &_settings; }

	static std::shared_ptr<Core::Node> SceneNode;
	static Core::Components::Data Components;
	static Utils::ThreadManager ThreadManager;
	static std::vector<Resources::IResource**> MultiThreadMeshes;
private:
	// Window
	static GLFWwindow* _window;
	static const GLFWvidmode* _videoMode;
	static bool _shouldClose;
	static Math::Matrix4 _VP;
	static GameState _gameState;
	static std::string _currentScenePath;
	double _lastFrame = 0;
	static double _deltaTime;

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
	static Utils::AppSettings _settings;

	// Render
	static Render::CameraEditor _cameraEditor;
	static Render::FrameBuffer _framebuffer;
	static Render::Gizmo _gizmo;

	void InitGlfw();
	void InitImGui();
	void InitGlad();
	void LoadResources();

	void FilesLoad(std::string path);

	void MultiThreadLoad();
	void PickingUpdate(std::vector<Core::Node*> nodes);
};

//TODO: Lights