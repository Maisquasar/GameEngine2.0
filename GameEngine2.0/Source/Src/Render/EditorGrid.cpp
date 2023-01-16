#include "Include/Render/EditorGrid.h"
#include "Include/Resources/Mesh.h"
#include "Include/Core/Components/MeshComponent.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include <ImGui/ImGuizmo/ImGuizmo.h>

Render::EditorGrid::EditorGrid()
{
	_meshComponent = new Core::Components::MeshComponent();
}

Render::EditorGrid::~EditorGrid()
{
	if (_meshComponent) {
		if (_meshComponent->GetMesh()->SubMeshes[0].Material)
			delete _meshComponent->GetMesh()->SubMeshes[0].Material;
		delete _meshComponent;
	}
}

void Render::EditorGrid::Initialize()
{
}

void Render::EditorGrid::Draw()
{
#if 0
	//static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	auto cam = Application.GetScene()->GetCameraEditor();
	auto VP = cam->GetViewMatrix() * cam->GetProjection();
	ImGui::Begin("Test");
	ImGuizmo::SetDrawlist();
	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
	ImGuizmo::DrawGrid(&cam->GetViewMatrix().content[0][0], &cam->GetViewMatrix().content[0][0], &Math::Mat4::Identity().content[0][0], 100.f);
	ImGui::End();
#else
	if (_meshComponent->GetMesh())
	{
		glDisable(GL_CULL_FACE);
		Math::Mat4 MVP = Application.GetScene()->GetVPMatrix();
		_meshComponent->GetMesh()->Update(MVP, false);
		glEnable(GL_CULL_FACE);
	}
	else
	{
		if (auto mesh = Application.GetResourceManager()->Get<Resources::Mesh>("Assets/Default/Models/Grid.obj::Grid"))
		{
			if (!mesh->Loaded)
				return;
			_meshComponent->SetMesh(dynamic_cast<Resources::Mesh*>(mesh->Clone()));
			_meshComponent->GetMesh()->SubMeshes[0].Material = new Resources::Material();
			for (auto sub : _meshComponent->GetMesh()->SubMeshes)
			{
				sub.Material->SetShader(Application.GetResourceManager()->Get<Resources::Shader>("Assets/Default/Shaders/GridShader"));
			}
			_meshComponent->GetMesh()->ShouldDrawCall = false;
		}
	}
#endif
}

