#pragma once
#include <vector>
#include <glad/glad.h>
#include "Include/Math/Math.h"
namespace Core::Components
{
	class MeshComponent;
}

namespace Render {
	class EditorGrid
	{
	public:
		EditorGrid();
		~EditorGrid();

		void Initialize();
		void Draw();

		Core::Components::MeshComponent* GetMeshComponent() { return _meshComponent; }
	private:
		Core::Components::MeshComponent* _meshComponent;
	};
}