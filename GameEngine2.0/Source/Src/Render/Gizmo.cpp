#include "Include/Render/Gizmo.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"

Render::Gizmo::Gizmo()
{
}

Render::Gizmo::~Gizmo()
{
	delete _mesh;
}

void Render::Gizmo::Draw()
{
	if (_mesh && NodeTransform)
	{
		if (!_mesh->IsInitialized())
			_mesh->Initialize();
		glBindVertexArray(_mesh->_VAO);
		float distance = App::GetEditorCamera()->Transform.GetLocalPosition().GetDistanceBetween(NodeTransform->GetWorldPosition());
		auto MVP = App::GetVPMatrix() * Math::Matrix4::CreateTransformMatrix(NodeTransform->GetWorldPosition(), NodeTransform->GetWorldRotation(), Math::Vector3(distance / 10));
		glDepthRange(0, 0.01);
		for (auto Sub : _mesh->SubMeshes)
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
	else if (!_mesh)
	{
		if (auto mesh = Resources::ResourceManager::Get<Resources::Mesh>("Assets/Default/Models/Gizmo.obj::Gizmo"))
		{
			_mesh = dynamic_cast<Resources::Mesh*>(mesh->Clone());
			for (auto Sub : _mesh->SubMeshes)
			{
				Sub.Material->SetShader(Resources::ResourceManager::GetDefaultShader());
			}
		}
	}
}
