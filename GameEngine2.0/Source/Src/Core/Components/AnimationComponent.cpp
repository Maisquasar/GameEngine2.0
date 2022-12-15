#include "Include/Core/Components/AnimationComponent.h"
#include "Include/Core/Components/SkeletalMesh.h"

#include "Include/Resources/Animation.h"
#include "Include/Resources/Skeleton.h"

#include "Include/App.h"

Core::Components::AnimationComponent::AnimationComponent()
{
	ComponentName = "AnimationComponent";
}

Core::Components::AnimationComponent::~AnimationComponent()
{
}

void Core::Components::AnimationComponent::Update()
{
	if (!_skeleton)
		_skeleton = GameObject->GetComponent<Core::Components::SkeletalMesh>();
	if (_currentAnimation && _skeleton)
	{
		if (_skeleton->Skeleton && _skeleton->Skeleton->RootBone)
		{
			_skeleton->Skeleton->RootBone->UpdateBone(_currentAnimation, _currentTime);
		}

		_currentTime += ImGui::GetIO().DeltaTime * 30;
		if (_currentTime > _currentAnimation->KeyRotCount)
			_currentTime = 0;
		else if (_currentTime <= 0)
			_currentTime = _currentAnimation->KeyRotCount;
	}
}

void Core::Components::AnimationComponent::ShowInInspector()
{
	if (ImGui::Button("Current Animation"))
	{
		ImGui::OpenPopup("AnimationPopup");
	}
	ImGui::SameLine();
	std::string name = "None";
	if (_currentAnimation)
		name = _currentAnimation->GetName();
	ImGui::Text(name.c_str());
	if (auto anim = Application.GetResourceManager()->ResourcesPopup< Resources::Animation>("AnimationPopup"))
	{
		SetCurrentAnimation(anim);
	}
}

void Core::Components::AnimationComponent::SetCurrentAnimation(Resources::Animation* a)
{
	_currentAnimation = a;
}
