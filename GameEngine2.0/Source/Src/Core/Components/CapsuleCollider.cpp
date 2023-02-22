#include "stdafx.h"
#include "..\..\..\Include\Core\Components\CapsuleCollider.h"

#include "Include\App.h"
#include "Include/Physic/PhysicHandler.h"
#include "Include/Physic/Physic.h"
#include "Include/Core/Node.h"
#include "Include/Core/Components/Rigidbody.h"
#include "Include/Utils/Loader.h"

Core::Components::CapsuleCollider::CapsuleCollider()
{
	ComponentName = "Capsule Collider";
}

Core::Components::CapsuleCollider::~CapsuleCollider()
{
	Application.GetPhysicHandler()->RemoveCollider(this);
}

void Core::Components::CapsuleCollider::Initialize()
{
	_shader = Application.GetResourceManager()->GetDefaultShader();

	_vertices[0] = Physic::GetDemiSphereVertices(_radius, true);
	_vertices[1] = Physic::GetCylindreVertices(_radius, 1.f);
	_vertices[2] = Physic::GetDemiSphereVertices(_radius, false);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

	for (int i = 0; i < 3; i++)
	{
		glGenVertexArrays(1, &_VAO[i]);
		glGenBuffers(1, &_VBO[i]);
		glBindVertexArray(_VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices[i].size(), _vertices[i].data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(POS, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(POS);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float[3])));
		glEnableVertexAttribArray(2);
	}

	Application.GetPhysicHandler()->AddCollider(this);
}

void Core::Components::CapsuleCollider::InitializePhysics()
{
	auto modelMatrix = GameObject->Transform.GetModelMatrix();
	auto worlpos = modelMatrix.GetPosition();
	auto rot = modelMatrix.GetEulerRotation();
	rot = Math::Vec3(rot.y, -rot.x, -rot.z + 90);
	auto quat = rot.ToQuaternion();
	auto transform = physx::PxTransform(physx::PxVec3(worlpos.x, worlpos.y, worlpos.z), physx::PxQuat(quat.x, quat.y, quat.z, quat.w).getConjugate());
	if (auto rb = GameObject->GetComponent<Core::Components::Rigidbody>()) {
		_dynamicBody = Application.GetPhysicHandler()->CreateDynamicCaspule(_radius, _height / 2, transform, _shape, _physicalMaterial->GetMaterial());
		rb->SetParameters(_dynamicBody);
	}
	else
		_staticBody = Application.GetPhysicHandler()->CreateStaticCaspule(_radius, _height / 2, transform, _shape, _physicalMaterial->GetMaterial());
}

void Core::Components::CapsuleCollider::EndPause()
{
	if (_dynamicBody) {
		auto pos = GameObject->Transform.GetWorldPosition();
		auto rot = GameObject->Transform.GetWorldRotation();
		_dynamicBody->setGlobalPose(physx::PxTransform(pos.x, pos.y, pos.z, physx::PxQuat(rot.x, rot.y, rot.z, rot.w)));
	}
}

void Core::Components::CapsuleCollider::GameUpdate()
{
	if (_dynamicBody) {
		auto pos = Math::Vec3(_dynamicBody->getGlobalPose().p.x, _dynamicBody->getGlobalPose().p.y, _dynamicBody->getGlobalPose().p.z);
		auto rot = Math::Quat(_dynamicBody->getGlobalPose().q.x, _dynamicBody->getGlobalPose().q.y, _dynamicBody->getGlobalPose().q.z, _dynamicBody->getGlobalPose().q.w).ToEuler();
		rot = Math::Vec3(rot.x, rot.y, rot.z + 90);
		GameObject->Transform.SetLocalPosition(pos);
		GameObject->Transform.SetLocalRotation(rot.ToQuaternion());
	}
}

using namespace Math;
using namespace Resources;
void Core::Components::CapsuleCollider::EditorDraw()
{
	if (!Application.GetSettings()->ShowCollider)
		return;
	glUseProgram(_shader->Program);
	// Draw Up Demi-Sphere.
	// Set the Model Matrix.
	auto pos = GameObject->Transform.GetWorldPosition();
	auto rot = GameObject->Transform.GetWorldRotation().ToEuler();
	Mat4 ObjModelMat = Mat4::CreateTransformMatrix(pos, rot, 1);
	Mat4 M = ObjModelMat * Mat4::CreateTransformMatrix(_center + Vec3(0, _height / 2, 0), Vec3(0), _radius);
	Mat4 MVP = Application.GetScene()->GetVPMatrix() * M;

	glUniform1i(_shader->GetLocation(Location::L_ENABLE_TEXTURE), false);
	// Send to the Shader.
	glUniformMatrix4fv(_shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);

	glUniform4f(_shader->GetLocation(Location::L_COLOR), 0, 1, 0, 1);


	int polygonMode;
	// Enable Wire frame.
	glDisable(GL_CULL_FACE);
	glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(_VAO[0]);
	glDraw(GL_TRIANGLES, 0, (GLsizei)_vertices[0].size() / 3);

	// Draw Cylinder.
	M = ObjModelMat * (Mat4::CreateTransformMatrix(_center, Vec3(0), Vec3(_radius, _height, _radius)));
	MVP = Application.GetScene()->GetVPMatrix() * M;

	// Send to the Shader.
	glUniformMatrix4fv(_shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniform4f(_shader->GetLocation(Location::L_COLOR), 0, 1, 0, 1);

	glBindVertexArray(_VAO[1]);
	glDraw(GL_TRIANGLES, 0, (GLsizei)_vertices[1].size() / 3);

	// Draw Down Sphere.
	M = ObjModelMat * (Mat4::CreateTransformMatrix(_center - Vec3(0, _height / 1.52f, 0), Vec3(0), _radius));
	MVP = Application.GetScene()->GetVPMatrix() * M;

	// Send to the Shader.
	glUniformMatrix4fv(_shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniform4f(_shader->GetLocation(Location::L_COLOR), 0, 1, 0, 1);

	glBindVertexArray(_VAO[2]);
	glDraw(GL_TRIANGLES, 0, (GLsizei)_vertices[2].size() / 3);

	//Disable Wire frame.
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
}

void Core::Components::CapsuleCollider::DrawPicking(int id)
{
	auto shader = Application.GetResourceManager()->GetPickingShader();
	if (!shader)
		return;
	glBindVertexArray(_VAO[0]);
	glUseProgram(shader->Program);
	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;

	auto pos = GameObject->Transform.GetWorldPosition();
	auto rot = GameObject->Transform.GetWorldRotation().ToEuler();
	Mat4 ObjModelMat = Mat4::CreateTransformMatrix(pos, rot, 1);
	Mat4 M = ObjModelMat * Mat4::CreateTransformMatrix(_center + Vec3(0, _height / 2, 0), Vec3(0), _radius);
	Mat4 MVP = Application.GetScene()->GetVPMatrix() * M;

	// Send to the Shader.
	glUniformMatrix4fv(shader->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniform4f(shader->GetLocation(Resources::Location::L_COLOR), r / 255.f, g / 255.f, b / 255.f, 1.f);

	glDraw(GL_TRIANGLES, 0, (GLsizei)_vertices[0].size() / 3);

	glBindVertexArray(_VAO[1]);
	M = ObjModelMat * (Mat4::CreateTransformMatrix(_center, Vec3(0), Vec3(_radius, _height, _radius)));
	MVP = Application.GetScene()->GetVPMatrix() * M;

	// Send to the Shader.
	glUniformMatrix4fv(shader->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniform4f(shader->GetLocation(Resources::Location::L_COLOR), r / 255.f, g / 255.f, b / 255.f, 1.f);

	glDraw(GL_TRIANGLES, 0, (GLsizei)_vertices[1].size() / 3);
	glBindVertexArray(_VAO[2]);

	// Draw Down Sphere.
	M = ObjModelMat * (Mat4::CreateTransformMatrix(_center - Vec3(0, _height / 1.52f, 0), Vec3(0), _radius));
	MVP = Application.GetScene()->GetVPMatrix() * M;

	// Send to the Shader.
	glUniformMatrix4fv(shader->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniform4f(shader->GetLocation(Resources::Location::L_COLOR), r / 255.f, g / 255.f, b / 255.f, 1.f);

	glDraw(GL_TRIANGLES, 0, (GLsizei)_vertices[2].size() / 3);
}

void Core::Components::CapsuleCollider::ShowInInspector()
{
	ImGui::DragFloat3("Center", &_center[0], 0.1f);
	ImGui::DragFloat("Height", &_height, 0.1f);
	ImGui::DragFloat("Radius", &_radius, 0.1f); 
	if (ImGui::Button("Change Physical Material"))
	{
		ImGui::OpenPopup("Physical Material Popup");
	}
	if (auto phm = Application.GetResourceManager()->ResourcesPopup<Resources::PhysicMaterial>("Physical Material Popup"))
	{
		_physicalMaterial = phm;
		if (_shape)
			_shape->setMaterials(phm->GetMaterials(), 1);
	}
	ImGui::SameLine();
	ImGui::TextUnformatted(_physicalMaterial->GetName().c_str());
}

void Core::Components::CapsuleCollider::SetUIIcon()
{
}

void Core::Components::CapsuleCollider::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Center : %s\n", this->_center.ToString().c_str());
	content += space + Utils::StringFormat("Radius : %f\n", this->_radius);
	content += space + Utils::StringFormat("Height : %f\n", this->_height);
	content += space + Utils::StringFormat("Material : %s\n", _physicalMaterial->GetPath().c_str());
}

void Core::Components::CapsuleCollider::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	while (currentLine.substr(0, 13) != "#EndComponent")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 6) == "Radius")
		{
			_radius = Utils::Loader::GetFloat(currentLine);
		}
		else if (currentLine.substr(0, 6) == "Height")
		{
			_height = Utils::Loader::GetFloat(currentLine);
		}
		else if (currentLine.substr(0, 6) == "Center")
		{
			_center = Utils::Loader::GetVector3(currentLine);
		}
		else if (currentLine.substr(0, 8) == "Material")
		{
			_physicalMaterial = Application.GetResourceManager()->Get<Resources::PhysicMaterial>(Utils::Loader::GetString(currentLine).c_str());
		}
		pos++;
	}
}
