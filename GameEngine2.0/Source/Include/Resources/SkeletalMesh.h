#pragma once
#include "Mesh.h"
namespace Resources {
	class SkeletalMesh : public Mesh
	{
	public:
		SkeletalMesh();
		~SkeletalMesh();

		void Initialize() override;
		void Update(Math::Matrix4 M, class Skeleton*, bool outline = false);
		void DrawPicking(Math::Matrix4 MVP, int id) override;

		SkeletalMesh* Clone() const {
			return new SkeletalMesh(static_cast<SkeletalMesh const&>(*this));
		}
	private:

	};
}