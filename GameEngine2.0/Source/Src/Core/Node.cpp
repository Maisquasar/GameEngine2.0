#include "Include/Core/Node.h"

Core::Node::Node()
{
	this->Transform.Parent = this->Parent.get();
	this->Transform.GameObject = this;
}

Core::Node::~Node(){}