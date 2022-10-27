#pragma once
#include "Component.h"

namespace Core::Component {
	class Mesh : public Component
	{
	public:
		Mesh();
		~Mesh();

		void ShowInInspector() override;

	private:

	};
}