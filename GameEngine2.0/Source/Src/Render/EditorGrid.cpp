#include "Include/Render/EditorGrid.h"
#include "Include/Resources/Mesh.h"
#include "Include/Core/Components/MeshComponent.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"

Render::EditorGrid::EditorGrid()
{
	_meshComponent = new Core::Components::MeshComponent();
}

Render::EditorGrid::~EditorGrid()
{
	delete _meshComponent;
}

void Render::EditorGrid::Initialize()
{
}

void Render::EditorGrid::Draw()
{
	if (_meshComponent->GetMesh())
	{
		glDisable(GL_CULL_FACE);
		Math::Matrix4 MVP = Application.GetScene()->GetVPMatrix() * Math::Matrix4::Identity();
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
			for (auto sub : _meshComponent->GetMesh()->SubMeshes)
			{
				sub.Material->SetShader(Application.GetResourceManager()->Get<Resources::Shader>("Assets/Default/Shaders/GridShader"));
			}
		}
	}
}

