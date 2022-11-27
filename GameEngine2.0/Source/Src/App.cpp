#include "Include/App.h"
#include <STB_Image/stb_image.h>
GLFWwindow* App::_window = nullptr;
const GLFWvidmode* App::_videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());;
bool App::_shouldClose = false;
Math::Matrix4 App::_VP;
std::shared_ptr<Core::Node> App::SceneNode = std::make_shared<Core::Node>();
Core::Components::Data App::Components;

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}
}

App::App() {}
App::App(const char* Name, int width, int height)
{
	_windowName = Name;
	_width = width; _height = height;
}
App::~App() {}

void App::InitializeApp()
{
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
	glfwSwapInterval(1); // Enable vsync
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
	// Load Shaders
	std::string path = "Assets/Default/Shaders";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		_resourceManager.Create<Resources::Shader>(entry.path().generic_string().data());
	}

	// Load Textures
	path = "Assets/Default/Textures";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		_resourceManager.Create<Resources::Texture>(entry.path().generic_string().data());
	}

	// Set-Up Default Material.
	auto defaultMat = new Resources::Material();
	defaultMat->SetEditable(false);
	defaultMat->SetName("DefaultMaterial");
	_resourceManager.Add<Resources::Material>("DefaultMaterial", defaultMat);

	// Load Material
	path = "Assets/Default/Models";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.path().string().substr(entry.path().string().find_last_of('.') + 1) == "mat")
			Utils::Loader::LoadMaterial(entry.path().generic_string().data());
	}

	// Load Models
	path = "Assets/Default/Models";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.path().string().substr(entry.path().string().find_last_of('.') + 1) == "obj")
			_resourceManager.Create<Resources::Model>(entry.path().generic_string().data());
	}
}

void App::Update()
{
	this->Components.Initialize();
	_framebuffer.Initialize(this->GetWindowSize());
	_editorUi.Initialize();
	SceneNode->AddChildren(new Core::Node());
	SceneNode->AddChildren(new Core::Node());
	SceneNode->AddChildren(new Core::Node());
	SceneNode->Childrens[0]->AddChildren(new Core::Node());
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// --------- Temporary ---------
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// --------- Temporary ---------
	_cameraEditor.Update();
	while (!glfwWindowShouldClose(_window) && !_shouldClose)
	{
		// Begin Frame
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		glfwGetFramebufferSize(_window, &_width, &_height);
		glBindFramebuffer(GL_FRAMEBUFFER, this->_framebuffer.FBO);
		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_DEPTH_BUFFER_BIT);
		// Begin Main Update

		if (_framebuffer.Window)
			_cameraEditor.AspectRatio = _framebuffer.Window->Size.x / _framebuffer.Window->Size.y;

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		SceneNode->UpdateSelfAndChilds();
		// --------- Temporary ---------
		auto shader = _resourceManager.Get<Resources::Shader>("Assets/Default/Shaders/UnlitShader");
		glUseProgram(shader->Program);
		_VP = _cameraEditor.GetProjection() * _cameraEditor.GetViewMatrix();
		auto MVP = _cameraEditor.GetProjection() * _cameraEditor.GetViewMatrix() * Math::Matrix4::CreateTransformMatrix(Math::Vector3(0), Math::Vector3(0), Math::Vector3(1));
		glUniformMatrix4fv(shader->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
		glUniform1i(shader->GetLocation(Resources::Location::L_ENABLE_TEXTURE), false);
		glUniform4f(shader->GetLocation(Resources::Location::L_COLOR), 1, 0.5f, 0.1f, 1);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// --------- Temporary ---------

		_editorUi.Draw();

		// Update Editor Camera.
		if (_framebuffer.UpdateCameraEditor) {
			_cameraEditor.Update();
			auto mousePos = ImGui::GetMousePos();
			// Set Mouse Pos Modulo Monitor Size
			if (mousePos.x < 1)
				SetCursorPos(this->GetMonitorVideoMode()->width - 2, (int)mousePos.y);
			if (mousePos.x >= this->GetMonitorVideoMode()->width - 1)
				SetCursorPos(2, (int)mousePos.y);
			if (mousePos.y < 1)
				SetCursorPos((int)mousePos.x, this->GetMonitorVideoMode()->height - 2);
			if (mousePos.y >= this->GetMonitorVideoMode()->height - 1)
				SetCursorPos((int)mousePos.x, 2);
		}

		_input.Update();

		_framebuffer.Draw();
		// End Main Update.
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
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
		// End Frame.
	}
}

void App::ClearApp()
{
	// Cleanup
	Components.Destroy();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(_window);
	glfwTerminate();
}

void App::CloseApp() { App::_shouldClose = true; }