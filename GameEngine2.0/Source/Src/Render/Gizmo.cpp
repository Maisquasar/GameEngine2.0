#include "Include/Render/Gizmo.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/Core/Components/MeshComponent.h"
#include "Include/Core/Node.h"
#include "Include/EditorUi/Editor.h"
#include "Include/EditorUi/Inspector.h"

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
			glDepthRange(0, 0.01);
			for (auto Sub : mesh->SubMeshes)
			{
				if (!Sub.Material || !Sub.Material->GetShader())
					continue;
				glUseProgram(Sub.Material->GetShader()->Program);
				glUniformMatrix4fv(Sub.Material->GetShader()->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
				glUniform1i(Sub.Material->GetShader()->GetLocation(Resources::Location::L_ENABLE_TEXTURE), Sub.Material->GetTexture() ? true : false);
				glUniform4f(Sub.Material->GetShader()->GetLocation(Resources::Location::L_COLOR), Sub.Material->GetDiffuse().x, Sub.Material->GetDiffuse().y, Sub.Material->GetDiffuse().z, Sub.Material->GetDiffuse().w);

				glDrawArrays(GL_TRIANGLES, (GLsizei)Sub.StartIndex, (GLsizei)Sub.Count);
			}
			glDepthRange(0.01, 1);
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
						delete MeshComp->GetMesh();
					}
					else
					{
						MeshComp = new Core::Components::MeshComponent();
						_axis[i]->AddComponent(MeshComp);
					}
					auto clonedMesh = dynamic_cast<Resources::Mesh*>(mesh->Clone());
					MeshComp->SetMesh(clonedMesh);
					for (auto Sub : clonedMesh->SubMeshes)
					{
						Sub.Material->SetShader(Application.GetResourceManager()->GetDefaultShader());
					}
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
	if (Application.GetFramebuffer()->IsHovered && !Application.GetFramebuffer()->UpdateCameraEditor) {
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
		auto mesh = dynamic_cast<Resources::Mesh*>(axis->GetComponent<Core::Components::MeshComponent>()->GetMesh());
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
		glDepthRange(0, 0.01);
		for (const auto& Sub : mesh->SubMeshes)
		{
			if (!Sub.Material)
				continue;
			glUniformMatrix4fv(Application.GetResourceManager()->GetPickingShader()->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
			glUniform4f(Application.GetResourceManager()->GetPickingShader()->GetLocation(Resources::Location::L_COLOR), r / 255.f, g / 255.f, b / 255.f, 1.f);

			glDrawArrays(GL_TRIANGLES, (GLsizei)Sub.StartIndex, (GLsizei)Sub.Count);
		}
		glDepthRange(0.01, 1);
		id++;
	}
}

void Render::Gizmo::Update(size_t ArrowClicked, Math::Vector2& mousePosition)
{
	if (ArrowClicked >= 0)
	{
		if (ArrowClicked < 3)
		{
			Math::Vector2 mouse = Application.GetFramebuffer()->GetMousePosition();
			mouse = mouse * Math::Vector2(Application.GetWindowSize().x / Application.GetFramebuffer()->GetSize().x, Application.GetWindowSize().y / Application.GetFramebuffer()->GetSize().y);
			auto currentMousePos = mouse;
			auto node = EditorUi::Editor::GetInspector()->NodesSelected[0];
			float difValue = (mousePosition[ArrowClicked % 2] - currentMousePos[ArrowClicked % 2]) / Math::Max(200 - (this->ForwardDistance * 5), 20);
			Math::Vector3 NewPosition;
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

Math::Matrix4 Render::Gizmo::GetMVP()
{
	// Project the points onto the custom axis
	auto axis = Application.GetScene()->GetCameraEditor()->Transform.GetForwardVector();
	float dot1 = NodeTransform->GetWorldPosition().DotProduct(axis);
	float dot2 = Application.GetScene()->GetCameraEditor()->Transform.GetLocalPosition().DotProduct(axis);
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
		return Application.GetScene()->GetVPMatrix() * Math::Matrix4::CreateTransformMatrix(M.GetWorldPosition(), Math::Vector3(), ForwardDistance / 10);
	}
}
