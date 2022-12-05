#pragma once
#include <vector>
#include "EditorWindow.h"

namespace EditorUi {
	class PerformanceWindow : public EditorWindow
	{
	public:
		PerformanceWindow();
		~PerformanceWindow();

		void Draw() override;
	private:
		std::vector<float> _fps;
		float _currentTime = 0;
	};
}