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

		static void AddNodeSelected(Core::Node* node);
		static void SetFileSelected(class File* file);

		static std::vector<Core::Node*> NodesSelected;
		static class File* FileSelected;

	private:

	};
}