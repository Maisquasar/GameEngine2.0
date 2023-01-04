#include "Include/App.h"

#include <glad/glad.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include "Include/Render/EditorGrid.h"
#include "Include/Physic/Physic.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/Debug/Line.h"
#include "Include/Core/Components/MeshComponent.h"
#include "Include/Render/CameraEditor.h"
#include "Include/Render/Framebuffer.h"
#include "Include/Core/Node.h"
#include "Include/Render/Gizmo.h"
#include "Include/Debug/Log.h"
#include "Include/Resources/Model.h"

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

	// Set-Up Default Resources.
	auto defaultMat = new Resources::Material();
	defaultMat->SetEditable(false);
	defaultMat->SetName("DefaultMaterial");
	defaultMat->SetShader(_resourceManager.GetDefaultShader());
	_resourceManager.Add<Resources::Material>("DefaultMaterial", defaultMat);

	auto LightMat = new Resources::Material();
	LightMat->SetEditable(false);
	LightMat->SetName("LightMat");
	LightMat->SetShader(_resourceManager.GetDefaultShader());
	_resourceManager.Add<Resources::Material>("LightMat", LightMat);

	auto CameraMat = new Resources::Material();
	CameraMat->SetEditable(false);
	CameraMat->SetName("CameraMat");
	CameraMat->SetShader(_resourceManager.GetDefaultShader());
	_resourceManager.Add<Resources::Material>("CameraMat", CameraMat);

	// Set-Up Default Plane.
	auto Plane = new Resources::Mesh();
	Plane->SetName("DefaultPlane");
	Plane->SetPath("DefaultPlane");
	Utils::GetPlaneVertices(Plane->Positions, Plane->TextureUVs, Plane->Normals, Plane->Indices);
	std::vector<float> _vertices;
	std::vector<unsigned int> _indices;
	Plane->VerticesLoop(_indices, _vertices);
	Plane->Vertices = _vertices;
	Plane->Initialize();
	auto sub = Resources::SubMesh();
	sub.Count = Plane->Indices.size();
	sub.StartIndex = 0;
	sub.Material = defaultMat;
	Plane->SubMeshes.push_back(sub);
	Plane->Loaded = true;

	_resourceManager.Add<Resources::Mesh>("DefaultPlane", Plane);

	// Load Textures - Materials - Models.
	FilesLoad("Assets");
	LightMat->SetTexture(_resourceManager.Get<Resources::Texture>("Assets/Default/Textures/LightIcon.png"));
	CameraMat->SetTexture(_resourceManager.Get<Resources::Texture>("Assets/Default/Textures/CameraIcon.png"));
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
	GetScene()->GetSceneNode()->Save("", content);
	_file.write(content.c_str(), content.size());
	_file.close();
}

void App::EndPlay()
{
	_gameState = GameState::Editor;
	auto file = "Assets/Default/Scenes/TemporaryScene.scene";
	GetScene()->LoadScene(file);
	std::filesystem::remove_all(file);
}

void App::MultiThreadLoad()
{
	// TODO: Move it to a update into resource manager
	// TODO: Optimize it (create a list with all uninitialize)
	if (_everythingIsLoaded)
		return;
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
		else
		{
			int x = 0;
		}
		total++;
	}
	total += this->MultiThreadMeshes.size();

	if (ImGui::Begin("##")) {
		float fraction = (float)loaded / (float)total;
		ImGui::ProgressBar(fraction);
	}
	ImGui::End();

	if (loaded == total)
		_everythingIsLoaded = true;

}
#pragma endregion

void App::BeginFrame()
{
	// Begin Frame
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	glfwGetFramebufferSize(_window, &_width, &_height);
	glBindFramebuffer(GL_FRAMEBUFFER, this->_framebuffer.FBO);
	glEnable(GL_DEPTH_TEST);
}

void App::Update()
{
	// Initilisation
	_lastFrame = 0;
	Components.Initialize();
	_editorUi.Initialize();
	_scene.Initialize();
	
	// Main loop
	while (!((glfwWindowShouldClose(_window) || _shouldClose) && _everythingIsLoaded))
	{
		BeginFrame();

		_scene.Update();

		_editorUi.Draw();

		_input.Update();

		_framebuffer.Draw();

#if MULTITHREAD_LOADING
		MultiThreadLoad();
#endif

		EndFrame();
	}
}

void App::EndFrame()
{
	// End Main Update.
	auto clearColor = _scene.GetClearColor();
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
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

void App::ClearApp()
{
	// Cleanup
	Components.Destroy();

	App::ThreadManager.Terminate();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(_window);
	glfwTerminate();
}

void App::CloseApp() { App::_shouldClose = true; }