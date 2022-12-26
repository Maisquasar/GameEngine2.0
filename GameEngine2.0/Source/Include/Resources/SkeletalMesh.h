#pragma once
#include "Mesh.h"
namespace Resources {
	class SkeletalMesh : public Mesh
	{
	public:
		SkeletalMesh();
		~SkeletalMesh();

		void Initialize() override;

	private:

	};
}