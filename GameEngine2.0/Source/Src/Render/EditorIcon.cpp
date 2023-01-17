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
	_material = Application.GetResourceManager()->Get<Resources::Material>(MatName.c_str());
	_material->SetShader(Application.GetResourceManager()->GetBillboardShader());
	Plane->SetSize(Math::Vec2(1, 1));
}

void Render::EditorIcon::Draw(Math::Mat4 VP, Core::Transform transform, bool selected)
{
	if (!Plane)
		return;
	std::vector<Resources::Material*> mat = { _material };
	Plane->Update(GetMVP(VP, transform), mat, selected);
}


void Render::EditorIcon::DrawPicking(Math::Mat4 VP, Core::Transform transform, int id)
{
	if (!Plane)
		return;
	std::vector<Resources::Material*> mat = { _material };
	Plane->DrawPicking(GetMVP(VP, transform), mat, id);
}

void Render::EditorIcon::SetSize(Math::Vec2 size)
{
	if (Plane) Plane->SetSize(size);
}

Math::Mat4 Render::EditorIcon::GetMVP(const Math::Mat4& VP, Core::Transform& transform)
{
	return VP * Math::Mat4::CreateTranslationMatrix(transform.GetWorldPosition());
}
