#include "Include/App.h"
GLFWwindow* App::_window = nullptr;
bool App::_shouldClose = false;

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	DebugLog(Debug::LogType::WARNING, "---------------");
	DebugLog(Debug::LogType::WARNING, "Debug message (%d): %s", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             DebugLog(Debug::LogType::WARNING, "Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   DebugLog(Debug::LogType::WARNING, "Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: DebugLog(Debug::LogType::WARNING, "Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     DebugLog(Debug::LogType::WARNING, "Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     DebugLog(Debug::LogType::WARNING, "Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           DebugLog(Debug::LogType::WARNING, "Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               DebugLog(Debug::LogType::WARNING, "Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: DebugLog(Debug::LogType::WARNING, "Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  DebugLog(Debug::LogType::WARNING, "Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         DebugLog(Debug::LogType::WARNING, "Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         DebugLog(Debug::LogType::WARNING, "Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              DebugLog(Debug::LogType::WARNING, "Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          DebugLog(Debug::LogType::WARNING, "Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           DebugLog(Debug::LogType::WARNING, "Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               DebugLog(Debug::LogType::WARNING, "Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         DebugLog(Debug::LogType::WARNING, "Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       DebugLog(Debug::LogType::WARNING, "Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          DebugLog(Debug::LogType::WARNING, "Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: DebugLog(Debug::LogType::WARNING, "Severity: notification"); break;
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
		DebugLog(Debug::LogType::L_ERROR, "Error Init GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	_window = glfwCreateWindow(_width, _height, _windowName, NULL, NULL);
	glfwSetScrollCallback(_window, Utils::Input::Scroll_Callback);
	if (_window == nullptr)
	{
		DebugLog(Debug::LogType::L_ERROR, "Failed to Create Window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(_window);
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
		DebugLog(Debug::LogType::L_ERROR, "Failed to initialize GLAD")
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
}

void App::Update()
{
	_framebuffer.Initialize();
	_editorUi.Initialize();
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// --------- Temporary ---------
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	// --------- Temporary ---------

	while (!glfwWindowShouldClose(_window) && !_shouldClose)
	{
		// Begin Frame
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		glBindFramebuffer(GL_FRAMEBUFFER, this->_framebuffer.FBO);
		glEnable(GL_DEPTH_TEST);
		glfwGetFramebufferSize(_window, &_width, &_height);
		glViewport(0, 0, _width, _height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_DEPTH_BUFFER_BIT);
		// Begin Main Update

		// --------- Temporary ---------
		auto shader = _resourceManager.Get<Resources::Shader>("UnlitShader");
		glUseProgram(shader->Program);
		auto MVP = _cameraEditor.GetProjection() * _cameraEditor.GetViewMatrix() * Math::Matrix4::CreateTransformMatrix(Math::Vector3(0), Math::Vector3(0), Math::Vector3(1));
		glUniformMatrix4fv(shader->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
		glUniform1i(shader->GetLocation(Resources::Location::L_ENABLE_TEXTURE), false);
		glUniform4f(shader->GetLocation(Resources::Location::L_COLOR), 1, 0.5, 0.1, 1);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// --------- Temporary ---------

		_editorUi.Draw();

		_input.Update();

		_cameraEditor.Update();

		_framebuffer.Draw();

		// End Main Update.

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
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(_window);
	glfwTerminate();
}

void App::CloseApp() { App::_shouldClose = true; }