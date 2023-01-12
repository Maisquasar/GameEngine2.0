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

		Math::Matrix4 Transform = Math::Matrix4::Identity();

	private:

	};
}