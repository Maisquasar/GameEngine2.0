#include "Include/Render/EditorIcon.h"
#include "Include/App.h"
#include "Include/Resources/Mesh.h"
#include "Include/Resources/BillBoard.h"
#include "Include/Utils/Utils.h"

Render::EditorIcon::EditorIcon()
{
}

Render::EditorIcon::~EditorIcon() { 
	if (Plane) 
		delete Plane; 
}

void Render::EditorIcon::Initialize(std::string MatName)
{
	//Plane = Cast(Resources::Mesh, Application.GetResourceManager()->GetDefaultPlane()->Clone());
	//auto mesh = Application.GetResourceManager()->Get<Resources::Mesh>("Assets/Default/Models/Mario.obj::Mario");
	auto mesh = Application.GetResourceManager()->GetDefaultPlane();
	Plane = new Resources::BillBoard(*static_cast<Resources::BillBoard*>(mesh));
	Plane->SubMeshes[0].Material = Application.GetResourceManager()->Get<Resources::Material>(MatName.c_str());
	Plane->SubMeshes[0].Material->SetShader(Application.GetResourceManager()->GetBillboardShader());
	Plane->SetSize(Math::Vector2(1, 1));
}

void Render::EditorIcon::Draw(Math::Matrix4 VP, Core::Transform transform)
{
	if (!Plane)
		return;
	Plane->Update(GetMVP(VP, transform), transform.GetWorldPosition());
}


void Render::EditorIcon::DrawPicking(Math::Matrix4 VP, Core::Transform transform, int id)
{
	Plane->DrawPicking(GetMVP(VP, transform), id);
}

Math::Matrix4 Render::EditorIcon::GetMVP(Math::Matrix4 VP, Core::Transform transform)
{
	return VP * transform.GetModelMatrix();
}
