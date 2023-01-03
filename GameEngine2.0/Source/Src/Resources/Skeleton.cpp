#include "Include/Resources/Skeleton.h"
#include "Include/Core/Components/SkeletalMeshComponent.h"
#include "Include/Debug/Line.h"
#include "Include/Debug/Log.h"

Bone::Bone() {
	if (!Transform.GameObject)
		Transform.GameObject = this;
}

Bone::~Bone() {
	// Remove it from the Skeletal Mesh.
	if (Parent) {
		if (auto skel = Parent->GetComponent<Core::Components::SkeletalMeshComponent>()) {
			if (skel->Skeleton && skel->Skeleton->RootBone == this)
			{
				delete skel->Skeleton;
				skel->Skeleton = nullptr;
			}
		}
	}
}

void Bone::ShowInInspector()
{
	if (ImGui::CollapsingHeader((Name + " " + std::to_string(Id)).c_str()))
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
	Math::Vector3 Position = DefaultPosition;
	Math::Quaternion Rotation = DefaultRotation;
	anim->GetAnimAtFrame(Id, time, Position, Rotation);

	Transform.SetLocalPosition(Position);
	Transform.SetLocalRotation(DefaultRotation * Rotation);

	for (auto child : Childrens)
		child->UpdateBone(anim, time);
}

void Bone::SetDefault()
{
	Transform.SetLocalPosition(DefaultPosition);
	Transform.SetLocalRotation(DefaultRotation);

	for (auto child : Childrens)
	{
		if (auto bone = dynamic_cast<Bone*>(child))
		{
			bone->SetDefault();
		}
	}
}

Bone* Bone::Clone()
{
	auto node = new Bone(static_cast<Bone const&>(*this));
	node->Childrens.clear();
	node->Components.clear();
	for (auto child : Childrens)
	{
		node->AddChildren(child->Clone());
	}
	for (auto child : Components)
	{
		node->AddComponent(child->Clone());
	}
	return node;
}

#include "Include/Utils/Utils.h"
void Bone::Save(std::string space, std::string& content)
{
	// Node Save.
	content += space + "#BeginBone\n";
	content += space + Utils::StringFormat("Id : %d\n", Id);
	content += space + Utils::StringFormat("DefaultPosition : %s\n", DefaultPosition.ToString().c_str());
	content += space + Utils::StringFormat("DefaultRotation : %s\n", DefaultRotation.ToString().c_str());
	content += space + Utils::StringFormat("Name : %s\n", Name.c_str());
	content += space + Utils::StringFormat("IsActive : %d\n", _active);

	// Components Save.
	Transform.Save(space, content);
	for (auto component : Components)
	{
		content += space + Utils::StringFormat("#BeginComponent %s\n", component->ComponentName.c_str());
		component->Save(space, content);
		content += space + Utils::StringFormat("#EndComponent %s\n", component->ComponentName.c_str());
	}

	// Children Save.
	for (auto children : Childrens)
	{
		children->Save(space + '\t', content);
	}
	content += space + "#EndBone\n";
}

#include "Include/Utils/Loader.h"
#include "Include/App.h"
#include "Include/Core/Components/SkeletalMeshComponent.h"
void Bone::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	while (currentLine.substr(0, 8) != "#EndBone")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 4) == "Name")
		{
			this->Name = Utils::Loader::GetString(currentLine);
		}
		else if (currentLine.substr(0, 8) == "IsActive")
		{
			this->_active = Utils::Loader::GetInt(currentLine);
		}
		else if (currentLine.substr(0, 2) == "Id")
		{
			this->Id = Utils::Loader::GetInt(currentLine);
		}
		else if (currentLine.substr(0, 15) == "DefaultPosition")
		{
			this->DefaultPosition = Utils::Loader::GetVector3(currentLine);
		}
		else if (currentLine.substr(0, 15) == "DefaultRotation")
		{
			this->DefaultRotation = Utils::Loader::GetVec4(currentLine);
		}
		else if (currentLine.substr(0, 8) == "Position")
		{
			this->Transform.SetLocalPosition(Utils::Loader::GetVector3(currentLine));
		}
		else if (currentLine.substr(0, 8) == "Rotation")
		{
			this->Transform.SetLocalRotation(Utils::Loader::GetVec4(currentLine));
		}
		else if (currentLine.substr(0, 5) == "Scale")
		{
			this->Transform.SetLocalScale(Utils::Loader::GetVector3(currentLine));
		}
		else if (currentLine.substr(0, 10) == "#BeginNode")
		{
			this->AddChildren(new Node());
			this->Childrens.back()->Load(data, pos);
		}
		else if (currentLine.substr(0, 10) == "#BeginBone")
		{
			this->AddChildren(new Bone());
			dynamic_cast<Bone*>(this->Childrens.back())->Load(data, pos);
		}
		else if (currentLine.substr(0, 15) == "#BeginComponent")
		{
			auto Type = Utils::Loader::GetString(currentLine);
			for (auto component : Application.Components.Components)
			{
				if (Type == component->ComponentName)
				{
					auto newComponent = component->Clone();
					newComponent->GameObject = this;
					newComponent->Load(data, pos);
					this->AddComponent(newComponent);
					break;
				}
			}
			continue;
		}
		else if (currentLine.substr(0, 8) == "#EndBone")
		{
			break;
		}
		pos++;
	}
	if (Parent && !dynamic_cast<Bone*>(Parent))
	{
		if (auto skel = Parent->GetComponent<Core::Components::SkeletalMeshComponent>())
			if (skel->Skeleton)
				skel->Skeleton->RootBone = this;
	}
}

std::vector<Bone*> Bone::GetAllBones()
{
	std::vector<Bone*> out;
	for (auto c : Childrens)
	{
		auto vec = dynamic_cast<Bone*>(c)->GetAllBones();
		out.insert(out.end(), vec.begin(), vec.end());
	}
	out.push_back(this);
	return out;
}

Math::Matrix4 Bone::GetBoneMatrix()
{
	auto result = Transform.GetModelMatrix(true) * DefaultMatrix;
	return result;
}

Resources::Skeleton::Skeleton() {}

Resources::Skeleton::~Skeleton()
{
	if (!RootBone->Parent)
		delete RootBone;
}

Resources::Skeleton* Resources::Skeleton::Clone() const
{
	// Clone The Skeleton.
	auto skel = dynamic_cast<Resources::Skeleton*>(BaseResource::Clone());
	// Clone All Bones.
	Bone* node = skel->RootBone->Clone();
	skel->RootBone = node;
	auto bones = skel->RootBone->GetAllBones();
	skel->Bones = bones;
	skel->SortBones();
	return skel;
}

std::vector<Math::Matrix4> Resources::Skeleton::GetBonesMatrices()
{
	std::vector<Math::Matrix4> Matrix;
	//Matrix.resize(52);
	for (size_t i = 0; i < Bones.size(); i++)
	{
		//Matrix.push_back(Math::Matrix4::Identity());
		Matrix.push_back(Bones[i]->GetBoneMatrix());
	}
	return Matrix;
}

bool compareById(Bone* a, Bone* b) {
	return a->Id < b->Id;
}

void Resources::Skeleton::SortBones()
{
	std::vector<Bone*> out;
	std::sort(Bones.begin(), Bones.end(), compareById);
}
