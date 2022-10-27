#pragma once
#include "../EditorUi/EditorWindow.h"
#include "Include/Core/Node.h"
namespace EditorUi
{
	class Inspector : public EditorWindow
	{
	public:
		Inspector();
		~Inspector();

		void Draw() override;

		static void AddSelected(Core::Node* node);

		static std::vector<Core::Node*> Selected;

	private:

	};
}