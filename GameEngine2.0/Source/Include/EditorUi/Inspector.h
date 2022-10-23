#pragma once
#include "../EditorUi/EditorWindow.h"
namespace EditorUi
{
	class Inspector : public EditorWindow
	{
	public:
		Inspector();
		~Inspector();

		void Draw() override;

	private:

	};
}