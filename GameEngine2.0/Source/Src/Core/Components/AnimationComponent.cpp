#include "Include/Core/Components/AnimationComponent.h"
#include "Include/Core/Components/SkeletalMeshComponent.h"

#include "Include/Resources/Animation.h"
#include "Include/Resources/Skeleton.h"
#include "Include/EditorUi/Editor.h"
#include "Include/EditorUi/AnimationWindow.h"
#include "Include/Utils/Loader.h"

#include "Include/App.h"

Core::Components::AnimationComponent::AnimationComponent()
{
	ComponentName = "AnimationComponent";
}

Core::Components::AnimationComponent::~AnimationComponent()
{
	if (_skeleton && _skeleton->Skeleton && _skeleton->Skeleton->RootBone)
	{
		_skeleton->Skeleton->RootBone->SetDefault();
	}
	if (EditorUi::Editor::GetAnimationWindow()->SelectedAnimationComp == this)
	{
		EditorUi::Editor::GetAnimationWindow()->SetOpen(false);
	}
}

void Core::Components::AnimationComponent::Update()
{
	if (!_skeleton)
		SetSkeleton(GameObject->GetComponent<Core::Components::SkeletalMeshComponent>());
	if (_currentAnimation && _skeleton)
	{
		if (_skeleton->Skeleton && _skeleton->Skeleton->RootBone)
		{
			_skeleton->Skeleton->RootBone->UpdateBone(_currentAnimation, _currentTime);
		}

		if (_play) {
			_currentTime += ImGui::GetIO().DeltaTime * 30;
			if (_currentTime > _currentAnimation->KeyCount)
				_currentTime = 0;
			else if (_currentTime <= 0)
				_currentTime = (float)_currentAnimation->KeyCount;
		}
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
	if (ImGui::Button("Show in Window")) {
		if (auto skelmesh = GameObject->GetComponent<Core::Components::SkeletalMeshComponent>())
			if (auto animComp = GameObject->GetComponent<Core::Components::AnimationComponent>())
				EditorUi::Editor::GetAnimationWindow()->SetAnimationAndSkeleton(animComp, skelmesh->Skeleton);
	}
}

void Core::Components::AnimationComponent::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Animation : %s\n", _currentAnimation ? _currentAnimation->GetPath().c_str() : "None");
}

void Core::Components::AnimationComponent::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	while (currentLine.substr(0, 13) != "#EndComponent")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 9) == "Animation")
		{
			auto AnimPath = Utils::Loader::GetString(currentLine);
			if (auto anim = Application.GetResourceManager()->Get<Resources::Animation>(AnimPath.c_str())) {
				_currentAnimation = anim;
			}
		}
		pos++;
	}
}

void Core::Components::AnimationComponent::SetCurrentAnimation(Resources::Animation* a)
{
	_currentAnimation = a;
}

void Core::Components::AnimationComponent::SetSkeleton(SkeletalMeshComponent* s)
{
	_skeleton = s;
}

void Core::Components::AnimationComponent::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/AnimationIcon.png");
}
