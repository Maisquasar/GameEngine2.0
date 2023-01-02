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

	// Clone this Bones and his childrens.
	Bone* Clone() override;

	void Save(std::string space, std::string& content) override;
	void Load(const char* data, uint32_t& pos) override;

	Math::Matrix4 GetBoneMatrix();

	int Id = -1;
	Math::Vector3 DefaultPosition;
	Math::Quaternion DefaultRotation;
	Math::Matrix4 DefaultMatrix;
private:

};
namespace Resources {
	class Skeleton : public BaseResource<Skeleton>
	{
	public:
		Skeleton();
		~Skeleton();

		Skeleton* Clone() const override;

		std::vector<Math::Matrix4> GetBonesMatrices();

		Bone* RootBone = nullptr;

		std::vector<Bone*> Bones;

		size_t BoneCount = 0;
	private:

	};
}