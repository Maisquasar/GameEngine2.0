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
	static Math::Integer2 GetWindowSize() 
	{
		int width, height;
		glfwGetWindowSize(_window, &width, &height);
		return Math::Integer2(width, height);
	}
	static const GLFWvidmode* GetMonitorVideoMode() 
	{
		if (!_videoMode)
			_videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		return _videoMode; 
	}
private:
	// Window
	static GLFWwindow* _window;
	static const GLFWvidmode* _videoMode;
	int _width = 800;
	int _height = 600;
	const char* _windowName = "GameEngine2.0";
	static bool _shouldClose;

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