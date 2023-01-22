#include "..\..\..\Include\Core\Components\SphereCollider.h"
#include <ImGui/imgui.h>

#include "Include\App.h"
#include "Include/Physic/PhysicHandler.h"
#include "Include/Physic/Physic.h"
#include "Include/Core/Node.h"
#include "Include/Core/Components/Rigidbody.h"

Core::Components::SphereCollider::SphereCollider()
{	
	ComponentName = "SphereCollider";
}

Core::Components::SphereCollider::~SphereCollider()
{
	Application.GetScene()->GetPhysicHandler()->RemoveCollider(this);
}

void Core::Components::SphereCollider::SetGameobject(Core::Node* node)
{
	Component::SetGameobject(node);
}

void Core::Components::SphereCollider::Initialize()
{
	_vertices = Physic::GetSphereVertices(1.f);

	_shader = Application.GetResourceManager()->GetDefaultShader();

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0U, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0U);

	glVertexAttribPointer(2U, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float[3])));
	glEnableVertexAttribArray(2U);

	Application.GetScene()->GetPhysicHandler()->AddCollider(this);
}

void Core::Components::SphereCollider::InitializePhysics()
{
	auto modelMatrix = GameObject->Transform.GetModelMatrix();
	auto worlpos = modelMatrix.GetPosition();
	auto quat = modelMatrix.GetRotation();
	auto transform = physx::PxTransform(physx::PxVec3(worlpos.x, worlpos.y, worlpos.z), physx::PxQuat(quat.x, quat.y, quat.z, quat.w).getConjugate());
	if (GameObject->GetComponent<Core::Components::Rigidbody>())
		_dynamicBody = Application.GetScene()->GetPhysicHandler()->CreateDynamicSphere(_radius, transform);
	else
		_staticBody = Application.GetScene()->GetPhysicHandler()->CreateStaticSphere(_radius, transform);
}

void Core::Components::SphereCollider::GameUpdate()
{
	if (_dynamicBody) {
		auto pos = Math::Vec3(_dynamicBody->getGlobalPose().p.x, _dynamicBody->getGlobalPose().p.y, _dynamicBody->getGlobalPose().p.z);
		auto rot = Math::Quat(_dynamicBody->getGlobalPose().q.x, _dynamicBody->getGlobalPose().q.y, _dynamicBody->getGlobalPose().q.z, _dynamicBody->getGlobalPose().q.w);
		GameObject->Transform.SetLocalPosition(pos);
		GameObject->Transform.SetLocalRotation(rot);
	}
}

void Core::Components::SphereCollider::Draw()
{
	glUseProgram(_shader->Program);
	// Set the Model Matrix.
	Math::Mat4 MVP = Application.GetScene()->GetVPMatrix() * GetModelMatrix();

	// Send to the Shader.
	glUniformMatrix4fv(_shader->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
	glUniform4f(_shader->GetLocation(Resources::Location::L_COLOR), 0, 1, 0, 1);
	glUniform1i(_shader->GetLocation(Resources::Location::L_ENABLE_TEXTURE), false);

	int polygonMode;
	// Enable Wire frame.
	glDisable(GL_CULL_FACE);
	glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(_VAO);
	glDraw(GL_TRIANGLES, 0, (GLsizei)_vertices.size() / 3);

	// Disable Wire frame.
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
}

void Core::Components::SphereCollider::UpdateTransform()
{
}

void Core::Components::SphereCollider::ShowInInspector()
{
	ImGui::DragFloat("Radius", &_radius, 0.1f);
}

void Core::Components::SphereCollider::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/SphereColliderIcon.png");
}

Math::Mat4 Core::Components::SphereCollider::GetModelMatrix()
{
	auto pos = GameObject->Transform.GetWorldPosition();
	auto rot = GameObject->Transform.GetWorldRotation().ToEuler();
	return Math::Mat4::CreateTransformMatrix(pos, rot, _radius);
}
