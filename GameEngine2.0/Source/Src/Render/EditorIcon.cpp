#include "Include/Render/EditorIcon.h"
#include "Include/App.h"
#include "Include/Resources/Mesh.h"
#include "Include/Utils/Utils.h"

Render::EditorIcon::EditorIcon()
{
	delete Plane;
}

Render::EditorIcon::~EditorIcon() { delete Plane; }

void Render::EditorIcon::Initialize(std::string MatName)
{
	Plane = Cast(Resources::Mesh, Application.GetResourceManager()->GetDefaultPlane()->Clone());
	Plane->SubMeshes[0].Material = Application.GetResourceManager()->Get<Resources::Material>(MatName.c_str());
}

void Render::EditorIcon::Draw(Math::Matrix4 VP, Core::Transform transform)
{
	Plane->Update(GetMVP(VP, transform));
}


void Render::EditorIcon::DrawPicking(Math::Matrix4 VP, Core::Transform transform, int id)
{
	Plane->DrawPicking(GetMVP(VP, transform), id);
}

Math::Matrix4 Render::EditorIcon::GetMVP(Math::Matrix4 VP, Core::Transform transform)
{
	auto rot = Application.GetScene()->GetCameraEditor()->Transform.GetLocalRotation() * Math::Vector3(-90, 0, 0).ToQuaternion();
	transform.SetLocalRotation(rot);

	transform.SetLocalScale(IconSize);
	return VP * transform.GetModelMatrix();
}
