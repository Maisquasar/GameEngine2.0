#include "Include/Core/Transform.h"
#include "Include/Core/Node.h"

Core::Transform::Transform(){}

Core::Transform::~Transform(){}

void Core::Transform::SetWorldPosition(Math::Vector3 pos)
{
	if (this->Parent)
		SetLocalPosition(pos - Parent->Transform.GetWorldPosition());
	else
		SetLocalPosition(pos);
}

void Core::Transform::SetWorldRotation(Math::Quaternion rot)
{
	if (Parent)
		// Set local rotation in function of parent rotation.
		SetLocalRotation(rot * Parent->Transform.GetWorldRotation().GetInverse());
	else
		SetLocalRotation(rot);
}

void Core::Transform::SetWorldScale(Math::Vector3 sca)
{
	if (Parent)
		// Set local scale in function of parent scale.
		SetLocalScale(Math::Vector3(sca.x / Parent->Transform.GetWorldScale().x, sca.y / Parent->Transform.GetWorldScale().y, sca.z / Parent->Transform.GetWorldScale().z));
	else
		SetLocalScale(sca);
}

Math::Vector3 Core::Transform::GetWorldPosition()
{
	return _modelMatrix.GetPosition();
}

Math::Quaternion Core::Transform::GetWorldRotation()
{
	return _modelMatrix.GetRotation().ToQuaternion();
}

Math::Vector3 Core::Transform::GetWorldScale()
{
	return _modelMatrix.GetScale();
}

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

void Core::Transform::RotateAround(Math::Vector3 point, Math::Vector3 axis, float angle)
{
		Math::Quaternion q = Math::Quaternion::AngleAxis(angle, axis);
		Math::Vector3 dif = this->GetWorldPosition() - point;
		dif = q * dif;
		this->SetWorldPosition(point + dif);
		this->SetWorldRotation(this->GetWorldRotation() * this->GetWorldRotation().GetInverse() * q * this->GetWorldRotation());
}
