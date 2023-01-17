#pragma once
#include <ImGui/imgui.h>
#include "Component.h"
namespace Resources
{
	class IResource;
	class Mesh;
	class Material;
}
namespace Render
{
	class EditorIcon;
}

namespace Core::Components {
	class MeshComponent : public BaseComponent<MeshComponent>
	{
	public:
		typedef BaseComponent<MeshComponent> Super;
		MeshComponent();
		~MeshComponent();

		void ShowInInspector() override;

		void Update() override;
		void Draw() override;
		void DrawPicking(int id) override;

		Resources::Mesh* GetMesh();
		void SetMesh(Resources::IResource* mesh);
		void SetUIIcon() override;
		void AddMaterial(Resources::Material* mat);

		std::vector<Resources::Material*>* GetMaterials() { return &_materials; }

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;
	private:
		Resources::IResource* _currentMesh;
		std::vector<Resources::Material*> _materials;
	};
}