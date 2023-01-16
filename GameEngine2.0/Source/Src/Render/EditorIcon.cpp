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
	auto mesh = Application.GetResourceManager()->GetDefaultPlane();
	Plane = new Resources::BillBoard(*static_cast<Resources::BillBoard*>(mesh));
	Plane->SubMeshes[0].Material = Application.GetResourceManager()->Get<Resources::Material>(MatName.c_str());
	Plane->SubMeshes[0].Material->SetShader(Application.GetResourceManager()->GetBillboardShader());
	Plane->SetSize(Math::Vec2(1, 1));
}

void Render::EditorIcon::Draw(Math::Mat4 VP, Core::Transform transform, bool selected)
{
	if (!Plane)
		return;
	Plane->Update(GetMVP(VP, transform), selected);
}


void Render::EditorIcon::DrawPicking(Math::Mat4 VP, Core::Transform transform, int id)
{
	Plane->DrawPicking(GetMVP(VP, transform), id);
}

void Render::EditorIcon::SetSize(Math::Vec2 size)
{
	if (Plane) Plane->SetSize(size);
}

Math::Mat4 Render::EditorIcon::GetMVP(const Math::Mat4& VP, Core::Transform& transform)
{
	return VP * transform.GetModelMatrix();
}
