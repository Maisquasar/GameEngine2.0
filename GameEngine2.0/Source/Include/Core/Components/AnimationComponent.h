#pragma once
#include "Component.h"
namespace Resources
{
	class Animation;
}
namespace EditorUi
{
	class AnimationWindow;
}
namespace Core::Components
{
	class AnimationComponent : public BaseComponent<AnimationComponent>
	{
	public:
		AnimationComponent();
		~AnimationComponent();

		void Update() override;
		void ShowInInspector() override;

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;

		void SetCurrentAnimation(Resources::Animation*);
		void SetSkeleton(class SkeletalMeshComponent*);

		Resources::Animation* GetCurrentAnimation() { return _currentAnimation; }
	private:
		Resources::Animation* _currentAnimation = nullptr;
		class SkeletalMeshComponent* _skeleton;
		float _currentTime = 0.0f;
		bool _play = false;

		friend class EditorUi::AnimationWindow;
	};
}