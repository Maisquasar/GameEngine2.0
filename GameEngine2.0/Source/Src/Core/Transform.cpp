#include "stdafx.h"
#include "Include/Core/Transform.h"
#include "Include/Core/Components/Component.h"
#include "Include/Core/Node.h"
#include "Include/Utils/Loader.h"
#include "Include/Resources/Texture.h"
#include "Include/App.h"

Core::Transform::Transform()
{
	ComponentName = "Transform";
	SetUIIcon();
}

Core::Transform::~Transform() {}

void Core::Transform::ComputeModelMatrix()
{
	_modelMatrix = GetLocalModelMatrix();
	_dirty = false;
}

void Core::Transform::ComputeModelMatrix(const Math::Mat4& parent)
{
	_modelMatrix = parent * GetLocalModelMatrix();
	_dirty = false;
}

void Core::Transform::SetWorldPosition(Math::Vec3 pos)
{
	if (!Parent)
		_localPosition = pos;
	else
		_localPosition = (_localPosition - Parent->Transform.GetWorldPosition()) / Parent->Transform.GetWorldScale();
	_dirty = true;
}

void Core::Transform::SetWorldRotation(Math::Quat rot)
{
	if (!Parent)
		_localRotation = rot;
	else
		_localRotation = rot * Parent->Transform.GetWorldRotation();
	_dirty = true;
}

void Core::Transform::SetWorldScale(Math::Vec3 sca)
{
	if (!Parent)
		_localScale = sca;
	else
		_localScale = _localScale / Parent->Transform.GetWorldScale();
	_dirty = true;
}

Math::Vec3 Core::Transform::GetWorldPosition()
{
	if (Parent)
		return Parent->Transform.GetModelMatrix() * (_localPosition);
	else
		return _localPosition;
}

Math::Quat Core::Transform::GetWorldRotation()
{
	if (Parent)
	{
		return Parent->Transform.GetWorldRotation() * _localRotation;
	}
	else
		return _localRotation;
}

Math::Vec3 Core::Transform::GetWorldScale()
{
	if (Parent)
	{
		return Parent->Transform.GetWorldScale() * _localScale;
	}
	else
		return _localScale;
}

Math::Mat4 Core::Transform::GetModelMatrix(bool update)
{
	if (_dirty || update) {
		if (Parent)
			return _modelMatrix = Parent->Transform.GetModelMatrix() * GetLocalModelMatrix();
		return _modelMatrix = GetLocalModelMatrix();
	}
	return _modelMatrix;
}

void Core::Transform::SetLocalPosition(Math::Vec3 newpos)
{
	_localPosition = newpos;
	_dirty = true;
}

void Core::Transform::SetLocalRotation(Math::Quat newrot)
{
	_localRotation = newrot;
	_localEulerRotation = newrot.ToEuler();
	_dirty = true;
}

void Core::Transform::SetLocalScale(Math::Vec3 newscale)
{
	_localScale = newscale;
	_dirty = true;
}

Math::Vec3 Core::Transform::GetLocalPosition()
{
	return _localPosition;
}

Math::Quat Core::Transform::GetLocalRotation()
{
	return _localRotation;
}

Math::Vec3 Core::Transform::GetLocalScale()
{
	return _localScale;
}

Math::Mat4 Core::Transform::GetLocalModelMatrix()
{
	return Math::Mat4::CreateTransformMatrix(_localPosition, _localRotation.ToEuler(), _localScale);
}

Math::Vec3 Core::Transform::GetForwardVector()
{
	return this->GetWorldRotation() * Math::Vec3::Forward();
}

Math::Vec3 Core::Transform::GetRightVector()
{
	return this->GetWorldRotation() * Math::Vec3::Right();
}

Math::Vec3 Core::Transform::GetUpVector()
{
	return this->GetWorldRotation() * Math::Vec3::Up();
}

Math::Vec3 Core::Transform::TranformPoint(Math::Vec3 p)
{
	return GetWorldRotation() * p;
}

void Core::Transform::RotateAround(Math::Vec3 point, Math::Vec3 axis, float angle)
{
	Math::Quat q = Math::Quat::AngleAxis(angle, axis);
	Math::Vec3 dif = GetWorldPosition() - point;
	dif = q * dif;
	SetWorldPosition(point + dif);
	SetWorldRotation(this->GetWorldRotation() * (this->GetWorldRotation().GetInverse() * q * this->GetWorldRotation()));
}

void Core::Transform::Update()
{
	if (!_dirty)
		return;
	ForceUpdate();
}

void Core::Transform::ForceUpdate()
{
	if (Parent)
		ComputeModelMatrix(Parent->Transform.GetModelMatrix());
	else
		ComputeModelMatrix();

	if (GameObject)
	{
		for (auto child : GameObject->Childrens)
		{
			child->Transform.ForceUpdate();
		}
		for (auto component : GameObject->Components)
		{
			component->UpdateTransform();
		}
	}
}

bool DrawVec3Control(const std::string& label, float* values, float resetValue = 0.0f, bool lockButton = false, float columnWidth = 100.0f)
{
	static bool _lock = false;
	bool stillEditing = false;
	if (lockButton && _lock)
	{
		float value = values[0]; 
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() * 3 - 15.f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// X
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values[0] = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		// y
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values[0] = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();

		// Z
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values[0] = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values[0], 0.1f, 0.0f, 0.0f, "%.2f"))
			stillEditing = true;
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		if (value != values[0]) {
			values[1] = values[0];
			values[2] = values[0];
		}
		ImGui::Columns(1);
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
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
	}
	if (lockButton) {
		ImGui::SameLine();
		ImGui::Checkbox("Lock", &_lock);
	}

	ImGui::PopID();
	return stillEditing;
}

void Core::Transform::ShowInInspector()
{
	Math::Vec3 position = _localPosition;
	Math::Vec3 rotation = _localEulerRotation;
	Math::Vec3 scale = _localScale;

	DrawVec3Control("Position", &position.x);
	DrawVec3Control("Rotation", &rotation.x);
	DrawVec3Control("Scale", &scale.x, 1.f, true);

	if (position != _localPosition || rotation != _localEulerRotation || scale != _localScale) {
		SetLocalPosition(position);
		_localEulerRotation = Math::Vec3(rotation.x, rotation.y, rotation.z);
		_localRotation = _localEulerRotation.ToQuaternion();
		SetLocalScale(scale);
		_dirty = true;
	}
}

void Core::Transform::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/TransformIcon.png");
}

void Core::Transform::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Position : %s\n", _localPosition.ToString().c_str());
	content += space + Utils::StringFormat("Rotation : %s\n", _localRotation.ToString().c_str());
	content += space + Utils::StringFormat("Scale : %s\n", _localScale.ToString().c_str());
}
