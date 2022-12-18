#pragma once
#include <Windows.h>
#include <vector>
#include "../EditorUi/EditorWindow.h"
namespace Core
{
	class Node;
}

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
		static void ClearSelected();
		bool IsSelected(Core::Node* node);

		static std::vector<Core::Node*> NodesSelected;
		static class File* FileSelected;

	private:

	};
}