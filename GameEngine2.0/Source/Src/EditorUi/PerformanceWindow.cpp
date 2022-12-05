#include "..\..\Include\EditorUi\PerformanceWindow.h"
#include <algorithm>

EditorUi::PerformanceWindow::PerformanceWindow()
{
}

EditorUi::PerformanceWindow::~PerformanceWindow()
{
}

const float Time = 0.1f;

void EditorUi::PerformanceWindow::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Performance", &_open))
	{
		if (_currentTime <= 0)
		{
			_currentTime = Time;
			if (_fps.size() < 60) {
				_fps.push_back(1 / ImGui::GetIO().DeltaTime);
			}
			else {
				_fps.erase(_fps.begin());
				_fps.push_back(1 / ImGui::GetIO().DeltaTime);
			}
		}
		else
		{
			_currentTime -= ImGui::GetIO().DeltaTime;
		}
		ImGui::PlotLines("##", &_fps[0], (int)_fps.size(), 0, NULL, 0.0f, FLT_MAX, ImVec2(0, 40));
		ImGui::BeginGroup();
		ImGui::Text("Fps Min : %.2f", *std::min_element(std::begin(_fps), std::end(_fps)));
		ImGui::Text("Fps Max : %.2f", *std::max_element(std::begin(_fps), std::end(_fps)));
		ImGui::EndGroup();
	}
	ImGui::End();
}
