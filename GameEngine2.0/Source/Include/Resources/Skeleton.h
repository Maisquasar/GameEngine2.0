#pragma once
#include "IResource.h"
#include "Include/Core/Transform.h"
#include "Include/Core/Node.h"

namespace Resources {
	class Animation;
}
class Bone : public Core::Node
{
public:
	Bone();
	~Bone();

	void ShowInInspector();
	void DrawDebug() override;
	void UpdateBone(Resources::Animation*, float) override;
	void SetDefault();

	int Id = -1;
	Math::Vector3 DefaultPosition;
	Math::Quaternion DefaultRotation;
private:

};
namespace Resources {
	class Skeleton : public BaseResource<Skeleton>
	{
	public:
		Skeleton();
		~Skeleton();

		Bone* RootBone = nullptr;

		size_t BoneCount = 0;
	private:

	};
}