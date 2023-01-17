#pragma once
#include "Component.h"
namespace Resources
{
	class Skeleton;
	class SkeletalMesh;
	class Material;
}
namespace Core::Components
{
	class SkeletalMeshComponent : public BaseComponent<SkeletalMeshComponent>
	{
	public:
		SkeletalMeshComponent();
		~SkeletalMeshComponent();

		void DrawPicking(int id) override;
		void Update() override;
		void ShowInInspector() override;

		void SetSkeleton(Resources::Skeleton* skel);
		void SetUIIcon() override;

		Resources::Skeleton* Skeleton = nullptr;
		Resources::SkeletalMesh* Mesh = nullptr;

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;

	private:
		bool _showMesh = true;
		bool _showSkeleton = true;
		std::vector<Resources::Material*> _materials;

	};
}