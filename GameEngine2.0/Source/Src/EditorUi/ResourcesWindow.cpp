#include "..\..\Include\EditorUi\ResourcesWindow.h"
#include "Include/Resources/ResourceManager.h"

EditorUi::ResourcesWindow::ResourcesWindow()
{
}

EditorUi::ResourcesWindow::~ResourcesWindow()
{
}

void EditorUi::ResourcesWindow::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Resources", &_open))
	{
		static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		if (ImGui::BeginTable("table_scrolly", 3, flags))
		{
			ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Path", ImGuiTableColumnFlags_None);
			ImGui::TableHeadersRow();
			int row = 0;
			for (auto res : Resources::ResourceManager::GetAllResources()) {
				ImGui::TableNextRow();
				for (int column = 0; column < 3; column++)
				{
					ImGui::TableSetColumnIndex(column);
					switch (column)
					{
					case 0:
						ImGui::Text("%s", typeid(*res.second).name());
						break;
					case 1:
						ImGui::Text(res.second->GetName().c_str());
						break;
					case 2:
						ImGui::Text(res.first.c_str());
						break;
					}
				}
				row++;
			}
			ImGui::EndTable();
		}

	}
	ImGui::End();
}
