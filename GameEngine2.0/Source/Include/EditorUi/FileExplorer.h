#pragma once
#include <string>
#include <vector>
#include <filesystem>

#include "FloatingFileExplorer.h"

namespace Resources
{
	class Texture;
	class IResource;
}

namespace EditorUi {
	class FileExplorer : public FloatingFileExplorer
	{
	public:
		FileExplorer();
		~FileExplorer();

		void Draw() override;
		void Refresh() override;
	};
}