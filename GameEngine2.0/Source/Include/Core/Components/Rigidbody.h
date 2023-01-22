#pragma once
#include "Component.h"
namespace Core::Components
{
	class Rigidbody : public BaseComponent<Rigidbody>
	{
	public:
		Rigidbody();
		~Rigidbody();

		void SetUIIcon();

	private:

	};
}