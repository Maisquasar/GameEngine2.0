#include "Include/Core/Transform.h"
#include "Include/Core/Node.h"

Core::Transform::Transform(){}

Core::Transform::~Transform(){}

void Core::Transform::ComputeModelMatrix()
{
	_modelMatrix = GetLocalModelMatrix();
	_dirty = false;
}

void Core::Transform::ComputeModelMatrix(Math::Matrix4 parent)
{
	_modelMatrix = GetLocalModelMatrix()* parent;
	_dirty = false;
}

void Core::Transform::SetWorldPosition(Math::Vector3 pos)
{
	if (!Parent)
		_localPosition = pos;
	else
		_localPosition = pos - Parent->Transform.GetWorldPosition();
	_dirty = true;
}

void Core::Transform::SetWorldRotation(Math::Quaternion rot)
{
	if (!Parent)
		_localRotation = rot;
	else
		_localRotation = (rot.ToEuler() - Parent->Transform.GetWorldRotation().ToEuler()).ToQuaternion();
	_dirty = true;
}

void Core::Transform::SetWorldScale(Math::Vector3 sca)
{
	if (!Parent)
		_localScale = sca;
	else
		_localScale = sca * Parent->Transform.GetWorldScale();
	_dirty = true;
}

Math::Vector3 Core::Transform::GetWorldPosition()
{
	if (Parent)
	{
		ForceUpdate();
		return _modelMatrix.GetPosition();
	}
	else
		return _localPosition;
}

Math::Quaternion Core::Transform::GetWorldRotation()
{
	if (Parent) 
	{
		ForceUpdate();
		return _modelMatrix.GetRotation();
	}
	else
		return _localRotation;
}

Math::Vector3 Core::Transform::GetWorldScale()
{
	if (Parent)
	{
		ForceUpdate();
		return _modelMatrix.GetScale();
	}
	else
		return _localScale;
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
	_localEulerRotation = newrot.ToEuler();
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

Math::Matrix4 Core::Transform::GetLocalModelMatrix()
{
	return Math::Matrix4::CreateTransformMatrix(_localPosition, _localRotation.ToEuler(), _localScale);
}

Math::Vector3 Core::Transform::GetForwardVector()
{
	return this->GetWorldRotation() * Math::Vector3::Forward();
}

Math::Vector3 Core::Transform::GetRightVector()
{
	return this->GetWorldRotation() * Math::Vector3::Right();
}

Math::Vector3 Core::Transform::GetUpVector()
{
	return this->GetWorldRotation() * Math::Vector3::Up();
}

void Core::Transform::RotateAround(Math::Vector3 point, Math::Vector3 axis, float angle)
{
	Math::Quaternion q = Math::Quaternion::AngleAxis(angle, axis);
	Math::Vector3 dif = GetWorldPosition() - point;
	dif = q * dif;
	SetWorldPosition(point + dif);
	SetWorldRotation(this->GetWorldRotation() * (this->GetWorldRotation().GetInverse() * q * this->GetWorldRotation()));
}

void Core::Transform::Update()
{
	if (!_dirty)
		return;
	ForceUpdate();
	//_modelMatrix = Math::Matrix4::CreateTransformMatrix(_localPosition, _localRotation, _localScale);
}

void Core::Transform::ForceUpdate()
{
	_localRotation = _localEulerRotation.ToQuaternion();
	if (Parent)
		ComputeModelMatrix(GameObject->Parent->Transform.GetModelMatrix());
	else
		ComputeModelMatrix();

	if (GameObject)
	{
		for (auto child : GameObject->Childrens)
		{
			child->Transform.ForceUpdate();
		}
	}
	//_modelMatrix = Math::Matrix4::CreateTransformMatrix(_localPosition, _localRotation, _localScale);
}

bool DrawVec3Control(const std::string& label, float* values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	bool stillEditing = false;

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values[0] = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::DragFloat("##X", &values[0], 0.1f, 0.0f, 0.0f, "%.2f"))
		stillEditing = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values[1] = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::DragFloat("##Y", &values[1], 0.1f, 0.0f, 0.0f, "%.2f"))
		stillEditing = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values[2] = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	if (ImGui::DragFloat("##Z", &values[2], 0.1f, 0.0f, 0.0f, "%.2f"))
		stillEditing = true;;
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
	return stillEditing;
}

void Core::Transform::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TreePush("##transform");
		Math::Vector3 position = _localPosition;
		Math::Vector3 rotation = _localEulerRotation;
		Math::Vector3 scale = _localScale;

		DrawVec3Control("Position", &position.x);
		DrawVec3Control("Rotation", &rotation.x);
		DrawVec3Control("Scale", &scale.x, 1.f);

		if (position != _localPosition || rotation != _localEulerRotation || scale != _localScale) {
			_localPosition = position;
			float mod = 360.f;
			_localEulerRotation = Math::Vector3(Math::Mod(rotation.x, mod), Math::Mod(rotation.y, mod), Math::Mod(rotation.z, mod));
			_localRotation.ToEuler().Print();
			_localScale = scale;
			_dirty = true;
		}
		ImGui::TreePop();
	}
}
