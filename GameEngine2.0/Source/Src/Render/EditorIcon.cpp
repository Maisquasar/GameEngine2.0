#include "Include/Render/EditorIcon.h"
#include "Include/App.h"
#include "Include/Resources/Mesh.h"
#include "Include/Resources/BillBoard.h"
#include "Include/Utils/Utils.h"

Render::EditorIcon::EditorIcon()
{
}

Render::EditorIcon::~EditorIcon() {
}

void Render::EditorIcon::Initialize(std::string MatName)
{
	auto mesh = Application.GetResourceManager()->GetDefaultPlane();
	Plane = mesh;
	_material = Application.GetResourceManager()->Get<Resources::Material>(MatName.c_str());
	_material->SetShader(Application.GetResourceManager()->GetBillboardShader());
	IconSize = { 1.f, 1.f };
}

void Render::EditorIcon::Draw(Math::Mat4 VP, Core::Transform transform, bool selected)
{
	if (!Plane)
		return;
	std::vector<Resources::Material*> mat = { _material };
	Plane->BillboardUpdate(GetMVP(VP, transform), mat, IconSize, selected);
}


void Render::EditorIcon::DrawPicking(Math::Mat4 VP, Core::Transform transform, int id)
{
	if (!Plane)
		return;
	std::vector<Resources::Material*> mat = { _material };
	Plane->BillboardPicking(GetMVP(VP, transform), mat, IconSize, id);
}

void Render::EditorIcon::SetSize(Math::Vec2 size)
{
	IconSize = size;
}

Math::Mat4 Render::EditorIcon::GetMVP(const Math::Mat4& VP, Core::Transform& transform)
{
	return VP * Math::Mat4::CreateTranslationMatrix(transform.GetWorldPosition());
}
