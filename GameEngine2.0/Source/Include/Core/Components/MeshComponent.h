#pragma once
#include "ImGui/imgui.h"
#include "Component.h"
#include "Include/Resources/Mesh.h"

namespace Core::Components {
	class MeshComponent : public BaseComponent<MeshComponent>
	{
	public:
		MeshComponent();
		~MeshComponent();

		void ShowInInspector() override;

		void Update() override;

		Resources::Mesh* GetMesh() { return _currentMesh; }

	private:
		Resources::Mesh* _currentMesh;
	};
}