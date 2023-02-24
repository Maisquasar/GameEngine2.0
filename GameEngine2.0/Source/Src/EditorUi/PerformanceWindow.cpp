#include "stdafx.h"
#include "..\..\Include\EditorUi\PerformanceWindow.h"

#include "Include/App.h"

EditorUi::PerformanceWindow::PerformanceWindow()
{
}

EditorUi::PerformanceWindow::~PerformanceWindow()
{
}

const float Time = 0.005f;

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
		ImGui::PlotHistogram("##", &_fps[0], (int)_fps.size(), 0, NULL, 0.0f, FLT_MAX, ImVec2(ImGui::GetWindowSize().x, 50));
		ImGui::BeginGroup();
		ImGui::Text("Fps Min : %.2f", *std::min_element(std::begin(_fps), std::end(_fps)));
		ImGui::Text("Fps Max : %.2f", *std::max_element(std::begin(_fps), std::end(_fps)));
		static float average = 0;
		auto value = Math::Mod((float)glfwGetTime(), 1.f);
		if (value <= 0.1f) {
			float sum = 0;
			for (int i = 0; i < _fps.size(); i++)
			{
				sum += _fps[i];
			}
			average = sum / _fps.size();
		}
		ImGui::Text("Average Fps : %.2f", average);
		ImGui::EndGroup();

		if (ImGui::TreeNode("Other Informations "))
		{
			char txt[32];
			snprintf(txt, 32, "OpenGL Version : %s", glGetString(GL_VERSION));
			ImGui::Text(txt);

			snprintf(txt, 32, "Vendor : %s", glGetString(GL_VENDOR));
			ImGui::Text(txt);

			snprintf(txt, 32, "GPU : %s", glGetString(GL_RENDERER));
			ImGui::Text(txt);
			ImGui::TreePop();
		}

		ImGui::Text("Triangles Draw : %d\nDraw Calls : %d\nInstances Draw : %d\nInstanced Triangles Draw : %d", 
			Application.GetSettings()->NumberOfTrianglesDraw, 
			Application.GetSettings()->NumberOfDrawCalls, 
			Application.GetSettings()->NumberOfInstances,
			Application.GetSettings()->NumberOfInstancedTriangles);
	}
	ImGui::End();
}
