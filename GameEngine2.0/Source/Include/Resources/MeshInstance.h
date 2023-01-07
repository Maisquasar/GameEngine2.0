#pragma once
#include "Mesh.h"

namespace Resources
{
	class MeshInstance : public Mesh
	{
	public:
		MeshInstance();
		~MeshInstance();

		void Initialize() override;

		void Draw(Shader* shader, int amount);

		Math::Matrix4 Transform = Math::Matrix4::Identity();

	private:

	};
}