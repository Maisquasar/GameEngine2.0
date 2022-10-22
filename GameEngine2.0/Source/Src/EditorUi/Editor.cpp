#include "Include/EditorUi/Editor.h"
EditorUi::FileExplorer EditorUi::Editor::_fileExplorer;

EditorUi::Editor::Editor(){}

EditorUi::Editor::~Editor(){}

void EditorUi::Editor::Draw()
{
	_mainBar.Draw();
	_hierarchy.Draw();
	_fileExplorer.Draw();
	Debug::Log::Console.Draw();
}