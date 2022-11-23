#pragma once
#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <filesystem>

#include "Include/Debug/Log.h"
#include "Include/EditorUi/Editor.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/Utils/Input.h"
#include "Include/Render/CameraEditor.h"
#include "Include/Render/Framebuffer.h"
#include "Include/Core/Node.h"
#include "Include/Resources/Model.h"

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

	static GLFWwindow* GetWindow() { return _window; }
	static Math::Integer2 GetWindowSize();
	static const GLFWvidmode* GetMonitorVideoMode();
	static std::shared_ptr<Core::Node> SceneNode;
	static Core::Components::Data Components;
	static Math::Matrix4 GetVPMatrix() { return _VP; }
private:
	// Window
	static GLFWwindow* _window;
	static const GLFWvidmode* _videoMode;
	int _width = 800;
	int _height = 600;
	const char* _windowName = "GameEngine2.0";
	static bool _shouldClose;
	static Math::Matrix4 _VP;

	// Ui
	EditorUi::Editor _editorUi;

	// Managers
	Resources::ResourceManager _resourceManager;

	// Utils
	Utils::Input _input;

	// Render
	Render::CameraEditor _cameraEditor;
	Render::FrameBuffer _framebuffer;
	
	void InitGlfw();
	void InitImGui();
	void InitGlad();
	void LoadResources();
};