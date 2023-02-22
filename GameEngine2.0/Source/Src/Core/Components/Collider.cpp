#include "stdafx.h"
#include "Include\Core\Components\Collider.h"
#include "Include\App.h"
#include "Include\Physic\PhysicHandler.h"

Core::Components::Collider::Collider()
{
	_physicalMaterial = Application.GetResourceManager()->GetDefaultPhysicMaterial();
}

Core::Components::Collider::~Collider()
{
}
