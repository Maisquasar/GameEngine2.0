#pragma once
#include "Mesh.h"
namespace Resources
{
	class BillBoard : public Mesh
	{
	public:
		BillBoard();
		~BillBoard();

		void Update(Math::Matrix4 MVP, bool outline = false) override;
		void DrawPicking(Math::Matrix4 MVP, int id) override;

		BillBoard* Clone() const {
			return new BillBoard(static_cast<BillBoard const&>(*this));
		}

		void SetSize(const Math::Vector2& size) { _size = size; }
	private:
		Math::Vector2 _size = Math::Vector2(32, 32);
	};
}