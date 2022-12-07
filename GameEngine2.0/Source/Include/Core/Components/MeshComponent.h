#pragma once
#include "ImGui/imgui.h"
#include "Component.h"
#include "Include/Resources/Mesh.h"

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

		Resources::Mesh* GetMesh() { return dynamic_cast<Resources::Mesh*>(_currentMesh); }
		void SetMesh(Resources::IResource* mesh) { _currentMesh = mesh; }

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;
	private:
		Resources::IResource* _currentMesh;
	};
}