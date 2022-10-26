#include "Include/Core/Transform.h"
#include "Include/Core/Node.h"

Core::Transform::Transform(){}

Core::Transform::~Transform(){}

void Core::Transform::SetWorldPosition(Math::Vector3 pos)
{
	_localPosition = pos;
	_modelMatrix.content[0][3] = pos.x;
	_modelMatrix.content[1][3] = pos.y;
	_modelMatrix.content[2][3] = pos.z;
}

void Core::Transform::SetWorldRotation(Math::Quaternion rot)
{
	_modelMatrix = Math::Matrix4::CreateTransformMatrix(GetWorldPosition(), rot.ToEuler(), GetWorldScale());
}

void Core::Transform::SetWorldScale(Math::Vector3 sca)
{
	_modelMatrix = Math::Matrix4::CreateTransformMatrix(GetWorldPosition(), GetWorldPosition(), sca);
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

Math::Matrix4 Core::Transform::GetModelMatrix()
{
	return _modelMatrix;
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
		Math::Vector3 dif = GetWorldPosition() - point;
		dif = q * dif;
		SetWorldPosition(point + dif);
		SetWorldRotation(this->GetWorldRotation() * this->GetWorldRotation().GetInverse() * q * this->GetWorldRotation());
}

void Core::Transform::Update()
{
	if (!_dirty)
		return;
	_modelMatrix = Math::Matrix4::CreateTransformMatrix(_localPosition, _localRotation.ToEuler(), _localScale);
}
