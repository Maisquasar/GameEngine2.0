#pragma once
#include "Component.h"

namespace Core::Components {
	class Mesh : public Component
	{
	public:
		Mesh();
		~Mesh();

		void ShowInInspector() override;

	private:

	};
}