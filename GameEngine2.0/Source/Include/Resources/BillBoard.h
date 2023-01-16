#pragma once
#include "Mesh.h"
namespace Resources
{
	class BillBoard : public Mesh
	{
	public:
		BillBoard();
		~BillBoard();

		void Update(Math::Mat4 MVP, bool outline = false) override;
		void DrawPicking(Math::Mat4 MVP, int id) override;

		BillBoard* Clone() const {
			return new BillBoard(static_cast<BillBoard const&>(*this));
		}

		void SetSize(const Math::Vec2& size) { _size = size; }
	private:
		Math::Vec2 _size = Math::Vec2(32, 32);
	};
}