#pragma once
#include "Mesh.h"
namespace Resources {
	class SkeletalMesh : public Mesh
	{
	public:
		SkeletalMesh();
		~SkeletalMesh();

		void Initialize() override;
		void Update(Math::Mat4 M, std::vector<class Material*>, class Skeleton*, bool outline = false);
		void DrawPicking(Math::Mat4 MVP, std::vector<class Material*>, Skeleton* skel, int id);

		SkeletalMesh* Clone() const {
			return new SkeletalMesh(static_cast<SkeletalMesh const&>(*this));
		}

	};
}