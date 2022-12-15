#pragma once
#include "Component.h"
namespace Resources
{
	class Animation;
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

		void SetCurrentAnimation(Resources::Animation*);
	private:
		Resources::Animation* _currentAnimation = nullptr;
		class SkeletalMesh* _skeleton;
		float _currentTime = 0.0f;

	};
}