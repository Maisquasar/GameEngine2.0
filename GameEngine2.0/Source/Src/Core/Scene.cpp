#include "stdafx.h"

#include "Include/Core/Scene.h"
#include "Include/Core/Node.h"
#include "Include/App.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/EditorUi/SceneWindow.h"
#include "Include/EditorUi/GameWindow.h"
#include "Include/Utils/Utils.h"
#include "Include/Utils/Loader.h"
#include "Include/Core/Components/CameraComponent.h"
#include "Include/Physic/PhysicHandler.h"

Core::Scene::Scene() {}

Core::Scene::~Scene()
{
	delete _sceneNode;
}

void Core::Scene::Initialize()
{
	_cameraEditor.Initialize();
	_cameraEditor.Update(true);
	LoadScene("Assets/Default/Scenes/DefaultScene.scene");
	_grid.Initialize();
}

void Core::Scene::BeginPlay()
{
}
#include "Include/Debug/Line.h"

void Core::Scene::Update()
{
	static Debug::Line line(10);

	// First Render form camera Editor.
	_sceneNode->UpdateSelfAndChilds();
	// Update Physic.
	Application.GetPhysicHandler()->Update();
	if (_cameraEditor.IsVisible()) {
		_currentCamera = &_cameraEditor;
		_VP = _cameraEditor.GetVP();
		_cameraEditor.PreUpdate();
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(_clearColor.x * _clearColor.w, _clearColor.y * _clearColor.w, _clearColor.z * _clearColor.w, _clearColor.w);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Begin Main Update
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Update Camera AspectRatio.
		if (EditorUi::Editor::GetSceneWindow()->Window) {
			_cameraEditor.AspectRatio = EditorUi::Editor::GetSceneWindow()->Window->Size.x / EditorUi::Editor::GetSceneWindow()->Window->Size.y;
		}

		// Debug Line
		static Math::Vec3 origin;
		static Math::Vec3 Direction;
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {

			origin = _currentCamera->GetTransform()->GetLocalPosition();
			Direction = _currentCamera->UnProject({ EditorUi::Editor::GetSceneWindow()->GetMousePosition(), 1000.f });
		}
		line.Draw(origin, Direction);

		// Draw Meshes with picking Shader.
		PickingUpdate(_sceneNode->GetAllChildrens());

		if (_showGrid)
			_grid.Draw();

		_sceneNode->DrawSelfAndChild(true);

		_gizmo.Draw();

		// Update Editor Camera.
		if (EditorUi::Editor::GetSceneWindow()->UpdateCameraEditor) {
			_cameraEditor.Update();
		}

		_cameraEditor.FrameBuffer.Draw();
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(_clearColor.x * _clearColor.w, _clearColor.y * _clearColor.w, _clearColor.z * _clearColor.w, _clearColor.w);
	glClear(GL_DEPTH_BUFFER_BIT);
	if (GetMainCamera() && EditorUi::Editor::GetGameWindow()->Window)
	{
		this->_mainCamera->AspectRatio = EditorUi::Editor::GetGameWindow()->Window->Size.x / EditorUi::Editor::GetGameWindow()->Window->Size.y;
	}
	// Render CameraComponents
	for (auto cam : Cameras)
	{
		if (!cam->IsVisible())
			return;
		_VP = cam->GetVP();
		_currentCamera = cam;
		RenderScene();
	}
}

void Core::Scene::RenderScene()
{
	_currentCamera->PreUpdate();
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(_clearColor.x * _clearColor.w, _clearColor.y * _clearColor.w, _clearColor.z * _clearColor.w, _clearColor.w);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Begin Main Update
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	_sceneNode->DrawSelfAndChild();

	_currentCamera->FrameBuffer.Draw();
}

void Core::Scene::PickingUpdate(std::vector<Core::Node*> nodes)
{
	static bool IsDown = false;
	static size_t ArrowClicked = -1;
	static Math::Vec2 mousePosition;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && EditorUi::Editor::GetSceneWindow()->IsHovered) {
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
		Math::Vec2 mouse = EditorUi::Editor::GetSceneWindow()->GetMousePosition();
		mouse = mouse * Math::Vec2(Application.GetWindowSize().x / EditorUi::Editor::GetSceneWindow()->GetSize().x, Application.GetWindowSize().y / EditorUi::Editor::GetSceneWindow()->GetSize().y);
		mouse.y = Application.GetWindowSize().y - mouse.y;
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
			Math::Vec2 mouse = EditorUi::Editor::GetSceneWindow()->GetMousePosition();
			mouse = mouse * Math::Vec2(Application.GetWindowSize().x / EditorUi::Editor::GetSceneWindow()->GetSize().x, Application.GetWindowSize().y / EditorUi::Editor::GetSceneWindow()->GetSize().y);
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

void Core::Scene::SaveScene()
{
	SaveNode(_currentScenePath, _sceneNode);
	PrintLog("Scene Saved at %s", _currentScenePath.c_str());
}

void Core::Scene::SaveNode(std::string Path, Core::Node* node)
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

void Core::Scene::LoadScene(std::string Path, bool temporary)
{
	if (EditorUi::Inspector().NodesSelected.size() > 0)
		EditorUi::Inspector().NodesSelected.clear();
	if (_gizmo.NodeTransform)
		_gizmo.NodeTransform = nullptr;

	if (_sceneNode)
		_sceneNode->RemoveAllChildrens();
	delete _sceneNode;
	if (!temporary)
		_currentScenePath = Path;
	_sceneNode = LoadNode(Path);
}

Core::Node* Core::Scene::LoadNode(std::string Path)
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

void Core::Scene::LoadTemporaryScene(std::string Path)
{
	if (EditorUi::Inspector().NodesSelected.size() > 0)
		EditorUi::Inspector().NodesSelected.clear();
	_sceneNode->RemoveAllChildrens();
	_sceneNode = LoadNode(Path);
}

void Core::Scene::SetCurrentCamera(Render::Camera* camera)
{
	_currentCamera = camera;
}

void Core::Scene::SetMainCamera(Render::Camera* camera)
{
	_mainCamera = camera;
}
