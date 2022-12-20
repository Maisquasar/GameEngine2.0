#include "Include/App.h"
#include <STB_Image/stb_image.h>
#include "Include/Render/EditorGrid.h"
#include "Include/Physic/Physic.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/Debug/Line.h"
#include "Include/Core/Components/MeshComponent.h"
App Application;

#pragma region Callbacks
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	LOG(Debug::LogType::WARNING, "---------------");
	LOG(Debug::LogType::WARNING, "Debug message (%d): %s", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             LOG(Debug::LogType::WARNING, "Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   LOG(Debug::LogType::WARNING, "Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: LOG(Debug::LogType::WARNING, "Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     LOG(Debug::LogType::WARNING, "Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     LOG(Debug::LogType::WARNING, "Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           LOG(Debug::LogType::WARNING, "Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               LOG(Debug::LogType::WARNING, "Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: LOG(Debug::LogType::WARNING, "Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  LOG(Debug::LogType::WARNING, "Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         LOG(Debug::LogType::WARNING, "Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         LOG(Debug::LogType::WARNING, "Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              LOG(Debug::LogType::WARNING, "Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          LOG(Debug::LogType::WARNING, "Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           LOG(Debug::LogType::WARNING, "Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               LOG(Debug::LogType::WARNING, "Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         LOG(Debug::LogType::WARNING, "Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG(Debug::LogType::WARNING, "Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          LOG(Debug::LogType::WARNING, "Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(Debug::LogType::WARNING, "Severity: notification"); break;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	// Resize the FrameBuffer.
	if (width * height != 0) {
		glActiveTexture(GL_TEXTURE0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}
}
#pragma endregion

#pragma region Initalisation

App::App() {}
App::App(const char* Name, int width, int height)
{
	_windowName = Name;
	_width = width; _height = height;
}
App::~App() {}

void App::InitializeApp(const char* Name, int width, int height)
{
	_windowName = Name;
	_width = width; _height = height;
	InitGlfw();
	InitGlad();
	InitImGui();
	LoadResources();
}

void App::InitGlfw()
{
	if (!glfwInit())
		LOG(Debug::LogType::L_ERROR, "Error Init GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	_window = glfwCreateWindow(_width, _height, _windowName, NULL, NULL);
	glfwSetScrollCallback(_window, Utils::Input::Scroll_Callback);
	if (_window == nullptr)
	{
		LOG(Debug::LogType::L_ERROR, "Failed to Create Window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

	// Disable V-Sync
	glfwSwapInterval(0);
}

void App::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void App::InitGlad()
{
	GLint flags = 0;
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG(Debug::LogType::L_ERROR, "Failed to initialize GLAD")
	}
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	//stbi_set_flip_vertically_on_load(true);
}

Math::Integer2 App::GetWindowSize()
{
	int width, height;
	glfwGetWindowSize(_window, &width, &height);
	return Math::Integer2(width, height);
}

const GLFWvidmode* App::GetMonitorVideoMode()
{
	if (!_videoMode)
		_videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	return _videoMode;
}

void App::LoadResources()
{
	_framebuffer.Initialize(this->GetWindowSize());

	// Load Shaders
	std::string path = "Assets/Default/Shaders";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		_resourceManager.Create<Resources::Shader>(entry.path().generic_string().data());
	}

	// Set-Up Default Material.
	auto defaultMat = new Resources::Material();
	defaultMat->SetEditable(false);
	defaultMat->SetName("DefaultMaterial");
	defaultMat->SetShader(_resourceManager.GetDefaultShader());
	_resourceManager.Add<Resources::Material>("DefaultMaterial", defaultMat);

	// Load Textures - Materials - Models.
	FilesLoad("Assets");
}

void App::FilesLoad(std::string path)
{
	auto dirIt = std::filesystem::directory_iterator(path);
	for (const auto& entry : dirIt) {
		auto ext = entry.path().string().substr(entry.path().string().find_last_of('.') + 1);
		if (entry.is_directory())
		{
			FilesLoad(entry.path().string());
		}
		else if (ext == "obj")
		{
			_resourceManager.Create<Resources::Model>(entry.path().generic_string().data());
		}
		else if (ext == "mat")
		{
			_resourceManager.Create<Resources::Material>(entry.path().generic_string().data());
		}
		else if (ext == "png" || ext == "jpg" || ext == "jpeg")
		{
			_resourceManager.Create<Resources::Texture>(entry.path().generic_string().data());
		}
		else if (ext == "fbx")
		{
			Utils::Loader::FBXLoad(entry.path().generic_string().data());
		}
	}
}

void App::BeginPlay()
{
	_gameState = GameState::Play;
	// Save Scene To Temporary Scene.
	std::ofstream _file;
	std::string filepath = "Assets/Default/Scenes/TemporaryScene.scene";
	_file.open(filepath);
	std::string content;
	SceneNode->Save("", content);
	_file.write(content.c_str(), content.size());
	_file.close();
}

void App::EndPlay()
{
	_gameState = GameState::Editor;
	auto file = "Assets/Default/Scenes/TemporaryScene.scene";
	LoadScene(file);
	std::filesystem::remove_all(file);
}

void App::MultiThreadLoad()
{
	// TODO: Move it to a update into resource manager
	// TODO: Optimize it (create a list with all uninitialize)
	if (_everythingIsLoaded)
		return;
	// Waiting 10 seconds until cancel it.
	if (currentFrame >= 10)
	{
		_everythingIsLoaded = true;
	}
	size_t loaded = 0, total = 0;
	for (auto res : _resourceManager.GetAllResources())
	{
		if (auto Texture = Cast(Resources::Texture, res.second))
		{
			if (!Texture->IsInitialized() && Texture->Loaded)
			{
				Texture->Initialize();
			}
		}
		else if (auto Mesh = Cast(Resources::Mesh, res.second))
		{
			if (!Mesh->IsInitialized() && Mesh->Loaded)
			{
				Mesh->Initialize();
				int ResIndex = 0;
				for (auto res : this->MultiThreadMeshes)
				{
					if ((*res)->GetPath() == Mesh->GetPath())
					{
						// Clone Mesh That was not loaded on scene loading.
						auto tmp = Cast(Resources::Mesh, Mesh->Clone());
						auto resMesh = Cast(Resources::Mesh, *res);
						int index = 0;
						for (auto Sub : tmp->SubMeshes)
						{
							Sub = resMesh->SubMeshes[index];
							index++;
						}
						delete (*res);
						(*res) = tmp;
						loaded++;
						MultiThreadMeshes.erase(MultiThreadMeshes.begin() + ResIndex);
					}
					ResIndex++;
				}
			}
		}
		if (res.second->IsInitialized())
			loaded++;
		total++;
	}
	total += this->MultiThreadMeshes.size();
	if (loaded == total)
		_everythingIsLoaded = true;

}
void App::PickingUpdate(std::vector<Core::Node*> nodes)
{
	static bool IsDown = false;
	static size_t ArrowClicked = -1;
	static Math::Vector2 mousePosition;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && GetFramebuffer()->IsHovered) {
		IsDown = true;
		int id = 0;
		size_t previousSize = nodes.size();

		for (auto child : nodes)
		{
			child->DrawPicking(id);
			id++;
		}
		_gizmo.DrawPicking(id);

		glFlush();
		glFinish();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char data[4];
		Math::Vector2 mouse = GetFramebuffer()->GetMousePosition();
		mouse = mouse * Math::Vector2(this->GetWindowSize().x / GetFramebuffer()->GetSize().x, this->GetWindowSize().y / GetFramebuffer()->GetSize().y);
		mouse.y = this->GetWindowSize().y - mouse.y;
		glReadPixels((GLint)mouse.x, (GLint)mouse.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		int pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256;

		if ((pickedID == 0x00ffffff || pickedID >= nodes.size() + 4 || pickedID < 0) && pickedID) {
			EditorUi::Inspector::ClearSelected();
		}
		else if (pickedID < nodes.size())
		{
			if (!ImGui::GetIO().KeyCtrl)
			{
				EditorUi::Inspector::ClearSelected();
			}
			EditorUi::Inspector::AddNodeSelected(nodes[pickedID]);
		}
		else
		{
			ArrowClicked = pickedID - nodes.size();
			Math::Vector2 mouse = GetFramebuffer()->GetMousePosition();
			mouse = mouse * Math::Vector2(this->GetWindowSize().x / GetFramebuffer()->GetSize().x, this->GetWindowSize().y / GetFramebuffer()->GetSize().y);
			mousePosition = mouse;
		}
		glClearColor(_clearColor.x * _clearColor.w, _clearColor.y * _clearColor.w, _clearColor.z * _clearColor.w, _clearColor.w);
		// Re-clear the screen for real rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else if (IsDown && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		IsDown = false;
		ArrowClicked = -1;
	}
	if (IsDown)
	{
		_gizmo.Update(ArrowClicked, mousePosition);
	}
}
#pragma endregion

void App::Update()
{
	// Initilisation
	_lastFrame = 0;
	Render::EditorGrid Grid;
	Grid.Initialize();
	Components.Initialize();
	_editorUi.Initialize();
	LoadScene("Assets/Default/Scenes/DefaultScene.scene");
	_cameraEditor.Update(true);

	// Main loop
	while (!glfwWindowShouldClose(_window) && !_shouldClose)
	{
#if MULTITHREAD_LOADING
		MultiThreadLoad();
#endif
		// Begin Frame
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		glfwGetFramebufferSize(_window, &_width, &_height);
		glBindFramebuffer(GL_FRAMEBUFFER, this->_framebuffer.FBO);
		glEnable(GL_DEPTH_TEST);

		if (this->_input.IsKeyPressed(ImGuiKey_F4))
		{
			PrintLog("Recompiling Shaders");
			_resourceManager.RecompileShaders();
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(_clearColor.x* _clearColor.w, _clearColor.y* _clearColor.w, _clearColor.z* _clearColor.w, _clearColor.w);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Begin Main Update

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Update Camera AspectRatio.
		if (_framebuffer.Window)
			_cameraEditor.AspectRatio = _framebuffer.Window->Size.x / _framebuffer.Window->Size.y;

		_VP = _cameraEditor.GetProjection() * _cameraEditor.GetViewMatrix();

		auto ChildList = SceneNode->GetAllChildrens();
		Utils::SortByDistanceFromCamera(ChildList, _cameraEditor.Transform.GetLocalPosition(), _cameraEditor.Transform.GetForwardVector());
		// Draw Meshs with picking Shader.
		PickingUpdate(ChildList);

		Grid.Draw();

		SceneNode->UpdateSelfAndChilds();

		_gizmo.Draw();

		_editorUi.Draw();

		// Update Editor Camera.
		if (_framebuffer.UpdateCameraEditor) {
			_cameraEditor.Update();
		}

		auto mouse = ImGui::GetMousePos();
		auto vecMouse = Math::Vector2(mouse.x, mouse.y) - _framebuffer.GetPos();

		_input.Update();

		_framebuffer.Draw();
		// End Main Update.
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(_clearColor.x * _clearColor.w, _clearColor.y * _clearColor.w, _clearColor.z * _clearColor.w, _clearColor.w);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Rendering.
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(_window);

		currentFrame = glfwGetTime();
		_deltaTime = currentFrame - _lastFrame;
		_lastFrame = currentFrame;
		// End Frame.
	}
}

void App::ClearApp()
{
	// Cleanup
	Components.Destroy();
	delete SceneNode;

	App::ThreadManager.Terminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(_window);
	glfwTerminate();
}

void App::CloseApp() { App::_shouldClose = true; }

#pragma region Load/Save

void App::LoadScene(std::string Path)
{
	if (EditorUi::Inspector().NodesSelected.size() > 0)
		EditorUi::Inspector().NodesSelected.clear();
	if (_gizmo.NodeTransform)
		_gizmo.NodeTransform = nullptr;

	if (SceneNode)
		SceneNode->RemoveAllChildrens();
	delete SceneNode;
	_currentScenePath = Path;
	SceneNode = LoadNode(Path);
}

Core::Node* App::LoadNode(std::string Path)
{
	auto node = new Core::Node();
	uint32_t size = 0;
	bool sucess;
	auto data = Utils::Loader::ReadFile(Path.c_str(), size, sucess);
	if (!sucess)
		nullptr;
	uint32_t pos = 0;
	// Skip First Line.
	Utils::Loader::SkipLine(data, pos);
	node->Load(data, pos);
	delete[] data;
	return node;
}

void App::LoadTemporaryScene(std::string Path)
{
	if (EditorUi::Inspector().NodesSelected.size() > 0)
		EditorUi::Inspector().NodesSelected.clear();
	SceneNode->RemoveAllChildrens();
	SceneNode = LoadNode(Path);
}

void App::SaveScene()
{
	SaveNode(_currentScenePath, SceneNode);
	PrintLog("Scene Saved at %s", _currentScenePath.c_str());
}
void App::SaveNode(std::string Path, Core::Node* node)
{
	// Save Scene To Temporary Scene.
	std::ofstream _file;
	_file.open(Path);
	if (_file) {
		std::string content;
		node->Save("", content);
		_file.write(content.c_str(), content.size());
		_file.close();
	}
}
#pragma endregion