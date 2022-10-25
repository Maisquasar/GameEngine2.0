#pragma once
#include "Transform.h"

namespace Core {
	class Node
	{
	public:
		Node();
		~Node();

		std::shared_ptr<Node> Parent;
		std::vector<std::shared_ptr<Node>> Childrens;
		Core::Transform Transform;

	private:

	};
}