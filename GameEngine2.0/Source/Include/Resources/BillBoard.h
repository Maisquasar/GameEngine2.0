#pragma once
#include "Mesh.h"
namespace Resources
{
	class BillBoard : public Mesh
	{
	public:
		BillBoard();
		BillBoard(Mesh mesh);
		~BillBoard();

		virtual void Update(Math::Mat4 MVP, std::vector<class Material*> materials, bool outline = false);
		virtual void DrawPicking(Math::Mat4 MVP, std::vector<class Material*> materials, int id);

		BillBoard* Clone() const {
			return new BillBoard(static_cast<BillBoard const&>(*this));
		}

		void SetSize(const Math::Vec2& size) { _size = size; }
	private:
		Math::Vec2 _size = Math::Vec2(32, 32);
	};
}