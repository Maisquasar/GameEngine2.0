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

void DrawVec3Control(const std::string& label, Math::Vector3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

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
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void Core::Transform::ShowInInspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TreePush("##transform");
		Math::Vector3 position = _localPosition;
		Math::Vector3 rotation = _localRotation.ToEuler();
		Math::Vector3 scale = _localScale;

		DrawVec3Control("Position", position);
		DrawVec3Control("Rotation", rotation);
		DrawVec3Control("Scale", scale, 1.f);

		if (position != _localPosition || rotation != _localRotation.ToEuler() || scale != _localScale) {
			_localPosition = position;
			_localRotation = rotation.ToQuaternion();
			_localScale = scale;
			_dirty = true;
		}
		ImGui::TreePop();
	}
}
