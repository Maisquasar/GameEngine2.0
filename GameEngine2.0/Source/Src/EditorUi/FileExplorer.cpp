#include "Include/EditorUi/FileExplorer.h"

// -------------- File --------------
EditorUi::File::File() {}

EditorUi::File::~File() {}

// -------------- File Explorer --------------

EditorUi::FileExplorer::FileExplorer() { _open = false; }

EditorUi::FileExplorer::~FileExplorer() {}

void EditorUi::FileExplorer::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("File Explorer", &_open))
	{

		ImGui::End();
	}
}