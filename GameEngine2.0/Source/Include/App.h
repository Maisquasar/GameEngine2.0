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

private:
	// Window
	GLFWwindow* _window = nullptr;
	int _width = 800;
	int _height = 600;
	const char* _windowName;
	static bool _shouldClose;

	// Ui
	EditorUi::Editor _editorUi;

	// Managers
	Resources::ResourceManager _resourceManager;

	void InitGlfw();
	void InitImGui();
	void InitGlad();
	void LoadResources();
};