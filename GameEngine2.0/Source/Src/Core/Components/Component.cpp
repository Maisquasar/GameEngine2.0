#include "Include\Core\Components\Component.h"
#include "Include/Core/Components/Mesh.h"
std::vector<Core::Components::Component> Core::Components::Data::Components;

Core::Components::Component::Component()
{
}

Core::Components::Component::~Component()
{
}

Core::Components::Data::Data()
{
}

Core::Components::Data::~Data()
{
}


void Core::Components::Data::NewComponent(Component comp)
{
	Components.push_back(comp);
}

void Core::Components::Data::Initialize()
{
	this->NewComponent(Core::Components::Mesh());
}
