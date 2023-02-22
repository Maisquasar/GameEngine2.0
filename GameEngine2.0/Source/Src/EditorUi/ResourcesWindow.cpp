#include "stdafx.h"
#include "..\..\Include\EditorUi\ResourcesWindow.h"
#include "Include/App.h"


EditorUi::ResourcesWindow::ResourcesWindow()
{
	if (!this->_resource)
		this->_resource = Application.GetResourceManager()->GetAllResourcesPtr();
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
			for (auto res : *_resource) {
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
						ImGui::TextUnformatted(res.second->GetName().c_str());
						break;
					case 2:
						ImGui::TextUnformatted(res.first.c_str());
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
