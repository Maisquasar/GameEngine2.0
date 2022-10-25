#include "Include/Core/Transform.h"

Core::Transform::Transform(){}

Core::Transform::~Transform(){}

void Core::Transform::SetLocalPosition(Math::Vector3 newpos)
{
	_localPosition = newpos;
	_dirty = true;
}

void Core::Transform::SetLocalRotation(Math::Quaternion newrot)
{
	_localRotation = newrot;
	_dirty = true;
}

void Core::Transform::SetLocalScale(Math::Vector3 newscale)
{
	_localScale = newscale;
	_dirty = true;
}

Math::Vector3 Core::Transform::GetLocalPosition()
{
	return _localPosition;
}

Math::Quaternion Core::Transform::GetLocalRotation()
{
	return _localRotation;
}

Math::Vector3 Core::Transform::GetLocalScale()
{
	return _localScale;
}
