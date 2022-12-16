#include "Include/Resources/Skeleton.h"
#include "Include/Debug/Line.h"
#include "Include/Debug/Log.h"

Bone::Bone() { if (!Transform.GameObject) Transform.GameObject = this; }

Bone::~Bone() {}

void Bone::ShowInInspector()
{
	if (ImGui::CollapsingHeader(Name.c_str()))
	{
		Transform.ShowInInspector();
		ImGui::TreePush(Name.c_str());
		for (auto child : Childrens)
		{
			dynamic_cast<Bone*>(child)->ShowInInspector();
		}
		ImGui::TreePop();
	}
}

void Bone::DrawDebug()
{
	for (auto child : Childrens)
	{
		auto pos = child->Transform.GetWorldPosition();
		auto pos2 = Transform.GetWorldPosition();

		Debug::Line line(pos, pos2, 1);
		line.Initialize();
		line.Draw();
		child->DrawDebug();
	}
}

#include "Include/Resources/Animation.h"
void Bone::UpdateBone(Resources::Animation* anim, float time)
{
	Math::Vector3 Position;
	Math::Quaternion Rotation;
	anim->GetAnimAtFrame(Id, time, Position, Rotation);

	Transform.SetLocalPosition(Position);
	Transform.SetLocalRotation(Rotation);

	Transform.Update();

	for (auto child : Childrens)
		if (auto c = dynamic_cast<Bone*>(child))
			c->UpdateBone(anim, time);
}



Resources::Skeleton::Skeleton() {}

Resources::Skeleton::~Skeleton() { if (!RootBone->Parent) delete RootBone; }

//TODO : 
//		- Fix ofbx.h leaks.
//		- Clean Loader Code.