#pragma once
#include "Component.h"
namespace Resources
{
	class Skeleton;
	class SkeletalMesh;
	class Material;
	class Mesh;
}
namespace Core::Components
{
	class SkeletalMeshComponent : public BaseComponent<SkeletalMeshComponent>
	{
	public:
		SkeletalMeshComponent();
		~SkeletalMeshComponent();

		void DrawPicking(int id) override;
		void Draw() override;
		void Update() override;
		void ShowInInspector() override;

		void SetSkeleton(Resources::Skeleton* skel);
		void SetMesh(Resources::Mesh* mesh);
		void SetUIIcon() override;

		Resources::Skeleton* Skeleton = nullptr;
		Resources::SkeletalMesh* Mesh = nullptr;

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;
		Core::Components::SkeletalMeshComponent* Clone() const override;

		void AddMaterial(Resources::Material* mat);
		std::vector<Resources::Material*>* GetMaterials() { return &_materials; }

	private:
		bool _showMesh = true;
		bool _showSkeleton = true;
		std::vector<Resources::Material*> _materials;

	};
}