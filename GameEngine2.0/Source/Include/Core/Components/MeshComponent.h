#pragma once
#include <ImGui/imgui.h>
#include "Component.h"
namespace Resources
{
	class IResource;
	class Mesh;
}

namespace Core::Components {
	class MeshComponent : public BaseComponent<MeshComponent>
	{
	public:
		typedef BaseComponent<MeshComponent> Super;
		MeshComponent();
		~MeshComponent();

		void ShowInInspector() override;

		void Draw();
		void DrawPicking(int id);

		Resources::Mesh* GetMesh();
		void SetMesh(Resources::IResource* mesh);

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;
	private:
		Resources::IResource* _currentMesh;
	};
}