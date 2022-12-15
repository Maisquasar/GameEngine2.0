#include "Include\Core\Components\Component.h"
#include "Include/Core/Components/MeshComponent.h"
#include "Include/Core/Components/BoxCollider.h"
#include "Include/Core/Components/SkeletalMesh.h"
#include "Include/Core/Components/AnimationComponent.h"
#include "Include/Core/Node.h"
std::vector<Core::Components::Component*> Core::Components::Data::Components;

Core::Components::Component::Component()
{
}

void Core::Components::Component::SetGameobject(Core::Node* node)
{
	GameObject = node;
}

void Core::Components::Component::RemoveFromParent()
{
	if (!GameObject)
		return;
	GameObject->RemoveComponent(this);
}

// ============ Data ========== //
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
	this->NewComponent(new Core::Components::BoxCollider());
	this->NewComponent(new Core::Components::SkeletalMesh());
	this->NewComponent(new Core::Components::AnimationComponent());
}
