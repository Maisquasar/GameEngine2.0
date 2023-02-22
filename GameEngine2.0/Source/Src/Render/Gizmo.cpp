#include "stdafx.h"
#include "Include/Render/Gizmo.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/Core/Components/MeshComponent.h"
#include "Include/Core/Node.h"
#include "Include/EditorUi/Editor.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/EditorUi/SceneWindow.h"

std::string MeshesName[] = {
	"::XAxisArrow",
	"::YAxisArrow",
	"::ZAxisArrow",
	"::Center",
};
std::string ModelsName[] = {
	"Assets/Default/Models/TranslateGizmo.obj",
	"",
	"Assets/Default/Models/ScaleGizmo.obj",
};
std::string MaterialsName[] = {
	"Assets/Default/Models/XAxis.mat",
	"Assets/Default/Models/YAxis.mat",
	"Assets/Default/Models/ZAxis.mat",
	"Assets/Default/Models/Center.mat",
};

Render::Gizmo::Gizmo()
{
	for (auto&& axis : _axis)
		axis = new Core::Node();
}

Render::Gizmo::~Gizmo()
{
	for (auto&& axis : _axis)
		delete axis;
}

void Render::Gizmo::Draw()
{
	if (_initialized && NodeTransform)
	{
		for (auto axis : _axis)
		{
			auto mesh = axis->GetComponent<Core::Components::MeshComponent>()->GetMesh();
			if (!mesh->IsInitialized())
				mesh->Initialize();
			glBindVertexArray(mesh->_VAO);

			auto MVP = GetMVP();
			glDepthRange(0.01, 0.02);
			auto meshcomp = axis->GetComponent<Core::Components::MeshComponent>();
			for (int i = 0; i < (*meshcomp->GetMaterials()).size(); i++)
			{
				auto mat = (*meshcomp->GetMaterials())[i];
				if (!mat || !mat->GetShader())
					continue;
				glUseProgram(mat->GetShader()->Program);
				glUniformMatrix4fv(mat->GetShader()->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
				glUniform1i(mat->GetShader()->GetLocation(Resources::Location::L_ENABLE_TEXTURE), mat->GetTexture() ? true : false);
				glUniform4f(mat->GetShader()->GetLocation(Resources::Location::L_COLOR), mat->GetDiffuse().x, mat->GetDiffuse().y, mat->GetDiffuse().z, mat->GetDiffuse().w);

				glDrawArrays(GL_TRIANGLES, (GLsizei)mesh->SubMeshes[i].StartIndex, (GLsizei)mesh->SubMeshes[i].Count);
			}
			glDepthRange(0.02, 1);
		}

	}
	else if (!_initialized)
	{
		int initializedNumber = 0;
		for (int i = 0; i < 4; i++)
		{
			if (!_foundMesh[i])
			{
				if (auto mesh = Application.GetResourceManager()->Get<Resources::Mesh>((ModelsName[(int)Mode] + MeshesName[i]).c_str()))
				{
					
					Core::Components::MeshComponent* MeshComp; 
					if (MeshComp = _axis[i]->GetComponent<Core::Components::MeshComponent>())
					{
					}
					else
					{
						MeshComp = new Core::Components::MeshComponent();
					}
					MeshComp->SetMesh(mesh);
					(*MeshComp->GetMaterials())[0] = Application.GetResourceManager()->Get<Resources::Material>(MaterialsName[i].c_str());
					_axis[i]->AddComponent(MeshComp);

					initializedNumber++;
				}
			}
			else
			{
				initializedNumber++;
			}
		}
		if (initializedNumber == 4)
			_initialized = true;
	}
	if (EditorUi::Editor::GetSceneWindow()->IsHovered && !EditorUi::Editor::GetSceneWindow()->UpdateCameraEditor) {
		if (Utils::Input::IsKeyPressed(ImGuiKey_W) && Mode != GizmoMode::Translate)
		{
			Mode = GizmoMode::Translate;
			_initialized = false;
			for (int i = 0; i < 4; i++) { _foundMesh[i] = false; }
		}
		else if (Utils::Input::IsKeyPressed(ImGuiKey_E) && Mode != GizmoMode::Rotate)
		{
			Mode = GizmoMode::Rotate;
			_initialized = false;
			for (int i = 0; i < 4; i++) { _foundMesh[i] = false; }
		}
		else if (Utils::Input::IsKeyPressed(ImGuiKey_R) && Mode != GizmoMode::Scale)
		{
			Mode = GizmoMode::Scale;
			_initialized = false;
			for (int i = 0; i < 4; i++) { _foundMesh[i] = false; }
		}
	}
}

void Render::Gizmo::DrawPicking(int id)
{
	if (!NodeTransform)
		return;
	for (auto axis : _axis)
	{
		auto meshComp = axis->GetComponent<Core::Components::MeshComponent>();
		auto mesh = meshComp->GetMesh();
		if (!mesh->IsInitialized())
			mesh->Initialize();
		if (!mesh->Loaded)
			return;
		if (!_initialized)
			mesh->Initialize();
		glBindVertexArray(mesh->_VAO);
		glUseProgram(Application.GetResourceManager()->GetPickingShader()->Program);
		int r = (id & 0x000000FF) >> 0;
		int g = (id & 0x0000FF00) >> 8;
		int b = (id & 0x00FF0000) >> 16;


		auto MVP = GetMVP();
		glDepthRange(0.01, 0.02);
		for (int i = 0; i < (*meshComp->GetMaterials()).size(); i++)
		{
			auto mat = (*meshComp->GetMaterials())[i];
			if (!mat)
				continue;
			glUniformMatrix4fv(Application.GetResourceManager()->GetPickingShader()->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
			glUniform4f(Application.GetResourceManager()->GetPickingShader()->GetLocation(Resources::Location::L_COLOR), r / 255.f, g / 255.f, b / 255.f, 1.f);

			glDrawArrays(GL_TRIANGLES, (GLsizei)mesh->SubMeshes[i].StartIndex, (GLsizei)mesh->SubMeshes[i].Count);
		}
		glDepthRange(0.02, 1);
		id++;
	}
}

void Render::Gizmo::Update(size_t ArrowClicked, Math::Vec2& mousePosition)
{
	if (ArrowClicked >= 0)
	{
		if (ArrowClicked < 3)
		{
			Math::Vec2 mouse = EditorUi::Editor::GetSceneWindow()->GetMousePosition();
			mouse = mouse * Math::Vec2(Application.GetWindowSize().x / EditorUi::Editor::GetSceneWindow()->GetSize().x, Application.GetWindowSize().y / EditorUi::Editor::GetSceneWindow()->GetSize().y);
			auto currentMousePos = mouse;
			auto node = EditorUi::Editor::GetInspector()->NodesSelected[0];
			float difValue = (mousePosition[ArrowClicked % 2] - currentMousePos[ArrowClicked % 2]) / Math::Max(200 - (this->ForwardDistance * 5), 20);
			Math::Vec3 NewPosition;
			NewPosition[ArrowClicked] = ArrowClicked == 2 ? -difValue : difValue;
			mousePosition = currentMousePos;
			switch (Mode)
			{
			case Render::GizmoMode::Translate:
				if (Application.GetSettings()->S_Transform == Utils::Settings::Transform::Local)
					NewPosition = this->NodeTransform->GetWorldRotation() * NewPosition;
				node->Transform.SetLocalPosition(node->Transform.GetLocalPosition() + NewPosition);
				break;
			case Render::GizmoMode::Rotate:
				break;
			case Render::GizmoMode::Scale:
				node->Transform.SetLocalScale(node->Transform.GetLocalScale() + NewPosition);
				break;
			default:
				break;
			}
		}

	}
}

Math::Mat4 Render::Gizmo::GetMVP()
{
	// Project the points onto the custom axis
	auto axis = Application.GetScene()->GetCameraEditor()->GetTransform()->GetForwardVector();
	float dot1 = NodeTransform->GetWorldPosition().DotProduct(axis);
	float dot2 = Application.GetScene()->GetCameraEditor()->GetTransform()->GetLocalPosition().DotProduct(axis);
	ForwardDistance = abs(dot1 - dot2);
	// Calculate the distance between the projected points

	if (Application.GetSettings()->S_Transform == Utils::Settings::Transform::Local)
	{
		auto M = *NodeTransform;
		M.SetLocalScale(ForwardDistance / 10);
		return Application.GetScene()->GetVPMatrix() * M.GetLocalModelMatrix();
	}
	else
	{
		auto M = *NodeTransform;
		return Application.GetScene()->GetVPMatrix() * (Math::Mat4::CreateTranslationMatrix(M.GetWorldPosition()) * Math::Mat4::CreateScaleMatrix(ForwardDistance / 10));
	}
}
