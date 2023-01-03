#pragma once
#include "../EditorUi/EditorWindow.h"
#include <unordered_map>

namespace Resources
{
	class IResource;
}

namespace EditorUi
{
	class ResourcesWindow : public EditorWindow
	{
	public:
		ResourcesWindow();
		~ResourcesWindow();

		void Draw() override;

	private:
		std::unordered_map<std::string, Resources::IResource*>* _resource;

	};
}