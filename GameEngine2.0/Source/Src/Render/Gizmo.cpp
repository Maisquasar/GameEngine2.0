#include "Include/Render/Gizmo.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/Core/Components/MeshComponent.h"
#include "Include/Core/Node.h"

const char* MeshesName[] = {
	"Assets/Default/Models/Gizmo.obj::XAxisArrow",
	"Assets/Default/Models/Gizmo.obj::YAxisArrow",
	"Assets/Default/Models/Gizmo.obj::ZAxisArrow",
	"Assets/Default/Models/Gizmo.obj::Center",
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
			auto mesh = dynamic_cast<Resources::Mesh*>(axis->GetComponent<Core::Components::MeshComponent>()->GetMesh());
			if (!mesh->IsInitialized())
				mesh->Initialize();
			glBindVertexArray(mesh->_VAO);
			// Project the points onto the custom axis
			auto axis = App::GetEditorCamera()->Transform.GetForwardVector();
			float dot1 = NodeTransform->GetWorldPosition().DotProduct(axis);
			float dot2 = App::GetEditorCamera()->Transform.GetLocalPosition().DotProduct(axis);
			ForwardDistance = abs(dot1 - dot2);
			// Calculate the distance between the projected points

			auto M = *NodeTransform;
			M.SetLocalScale(ForwardDistance / 10);
			auto MVP = App::GetVPMatrix() * M.GetLocalModelMatrix();
			glDepthRange(0, 0.01);
			for (auto Sub : mesh->SubMeshes)
			{
				if (!Sub.Material)
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
				if (auto mesh = Resources::ResourceManager::Get<Resources::Mesh>(MeshesName[i]))
				{
					auto MeshComp = new Core::Components::MeshComponent();
					_axis[i]->AddComponent(MeshComp);
					auto clonedMesh = dynamic_cast<Resources::Mesh*>(mesh->Clone());
					MeshComp->SetMesh(clonedMesh);
					for (auto Sub : clonedMesh->SubMeshes)
					{
						Sub.Material->SetShader(Resources::ResourceManager::GetDefaultShader());
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
		glUseProgram(Resources::ResourceManager::GetPickingShader()->Program);
		int r = (id & 0x000000FF) >> 0;
		int g = (id & 0x0000FF00) >> 8;
		int b = (id & 0x00FF0000) >> 16;

		float distance = App::GetEditorCamera()->Transform.GetLocalPosition().GetDistanceBetween(NodeTransform->GetWorldPosition());
		auto MVP = App::GetVPMatrix() * Math::Matrix4::CreateTransformMatrix(NodeTransform->GetWorldPosition(), NodeTransform->GetWorldRotation(), Math::Vector3(distance / 10));

		glDepthRange(0, 0.01);
		for (const auto& Sub : mesh->SubMeshes)
		{
			if (!Sub.Material)
				continue;
			glUniformMatrix4fv(Resources::ResourceManager::GetPickingShader()->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
			glUniform4f(Resources::ResourceManager::GetPickingShader()->GetLocation(Resources::Location::L_COLOR), r / 255.f, g / 255.f, b / 255.f, 1.f);

			glDrawArrays(GL_TRIANGLES, (GLsizei)Sub.StartIndex, (GLsizei)Sub.Count);
		}
		glDepthRange(0.01, 1);
		id++;
	}
}
