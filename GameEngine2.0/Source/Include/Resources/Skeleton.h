#pragma once
#include "IResource.h"
#include "Include/Debug/Line.h"
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
	Bone* CloneNode() override;

	void Save(std::string space, std::string& content) override;
	void Load(const char* data, uint32_t& pos) override;

	std::vector<Bone*> GetAllBones();

	Math::Mat4 GetBoneMatrix();

	int Id = -1;
	Math::Vec3 DefaultPosition;
	Math::Quat DefaultRotation;
	Math::Mat4 DefaultMatrix;

	Debug::Line DebugLine;
private:

};
namespace Resources {
	class Skeleton : public BaseResource<Skeleton>
	{
	public:
		Skeleton();
		~Skeleton();

		Skeleton* Clone() const override;

		std::vector<Math::Mat4> GetBonesMatrices();
		void SortBones();

		Bone* RootBone = nullptr;

		// sorted list by index
		std::vector<Bone*> Bones;

		size_t BoneCount = 0;

		void SetMaxBoneWeight(int i) { _maxBoneWeight = i; }
		int GetMaxBoneWeight() { return _maxBoneWeight; }
	private:
		int _maxBoneWeight = 0;

	};
}