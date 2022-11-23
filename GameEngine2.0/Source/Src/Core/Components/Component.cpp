#include "Include\Core\Components\Component.h"
#include "Include/Core/Components/MeshComponent.h"
std::vector<Core::Components::Component*> Core::Components::Data::Components;

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
	Destroy();
}

void Core::Components::Data::Destroy()
{
	for (auto component : Components)
	{
		delete component;
	}
}


void Core::Components::Data::NewComponent(Component* comp)
{
	Components.push_back(comp);
}

void Core::Components::Data::Initialize()
{
	this->NewComponent(new Core::Components::MeshComponent());
}
