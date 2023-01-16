#pragma once
#include "Mesh.h"

namespace Resources
{
	class MeshInstance : public Mesh
	{
	public:
		MeshInstance();
		~MeshInstance();

		void Initialize(int buf = -1);

		void Draw(int amount);

		Math::Mat4 Transform = Math::Mat4::Identity();

	private:

	};
}