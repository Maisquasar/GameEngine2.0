#pragma once
#include "ImGui/imgui.h"
#include "Component.h"

namespace Core::Components {
	class MeshComponent : public Components::Component
	{
	public:
		MeshComponent();
		~MeshComponent();

		void ShowInInspector() override;

	private:

	};
}