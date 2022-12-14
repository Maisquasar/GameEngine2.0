#pragma once
#include "Component.h"
namespace Resources
{
	class Skeleton;
	class Mesh;
}
namespace Core::Components
{
	class SkeletalMesh : public BaseComponent<SkeletalMesh>
	{
	public:
		SkeletalMesh();
		~SkeletalMesh();

		void Update() override;
		void ShowInInspector() override;

		Resources::Skeleton* Skeleton = nullptr;
		Resources::Mesh* Mesh = nullptr;

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;
	private:

	};
}