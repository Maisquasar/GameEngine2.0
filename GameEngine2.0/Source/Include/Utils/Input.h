#pragma once
#include "Include/Math/Math.h"

namespace Utils {
	class Input
	{
	public:
		Input();
		~Input();

		static float MouseScroll;
		static Math::Vec2 MouseDelta;
		static Math::Vec2 MousePosition;

		static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);

		void Update();

		static bool IsKeyDown(int key);
		static bool IsKeyPressed(int key);
		static bool IsKeyReleased(int key);

	private:

	};
}