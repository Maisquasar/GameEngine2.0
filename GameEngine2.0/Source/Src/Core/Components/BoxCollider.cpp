#include "Include/Core/Components/BoxCollider.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>

#include "Include/Math/Math.h"
#include "Include/Physic/Physic.h"
#include "Include/Physic/PhysicHandler.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/Physic/Ray.h"
#include "Include/Debug/Line.h"
#include "Include/Core/Transform.h"
#include "Include/Core/Node.h"
#include "Include/Core/Components/Rigidbody.h"
#include "Include/Utils/Loader.h"


Core::Components::BoxCollider::BoxCollider()
{
	ComponentName = "BoxCollider";
}

Core::Components::BoxCollider::~BoxCollider()
{
	Application.GetScene()->GetPhysicHandler()->RemoveCollider(this);
}

void Core::Components::BoxCollider::SetGameobject(Core::Node* node)
{
	Super::SetGameobject(node);
}

void Core::Components::BoxCollider::Initialize()
{
	_vertices = Physic::GetCubeVertices();

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

void Core::Components::BoxCollider::InitializePhysics()
{
	auto modelMatrix = GameObject->Transform.GetModelMatrix();
	auto worlpos = modelMatrix.GetPosition();
	auto quat = modelMatrix.GetRotation();
	auto scale = modelMatrix.GetScale();
	auto transform = physx::PxTransform(physx::PxVec3(worlpos.x, worlpos.y, worlpos.z), physx::PxQuat(quat.x, quat.y, quat.z, quat.w).getConjugate());
	if (auto rb = GameObject->GetComponent<Core::Components::Rigidbody>())
	{
		_dynamicBody = Application.GetScene()->GetPhysicHandler()->CreateDynamicCube(scale * _extent, transform, rb->GetMass());
		auto vel = rb->GetInitialVelocity();
		_dynamicBody->addForce({ vel.x, vel.y, vel.z }, physx::PxForceMode::eVELOCITY_CHANGE);
	}
	else
		_staticBody = Application.GetScene()->GetPhysicHandler()->CreateStaticCube(scale * _extent, transform);
}

void Core::Components::BoxCollider::Draw()
{
	glUseProgram(_shader->Program);
	// Set the Model Matrix.
	Math::Mat4 M = this->GameObject->Transform.GetModelMatrix() * Math::Mat4::CreateScaleMatrix(_extent);
	Math::Mat4 MVP = Application.GetScene()->GetVPMatrix() * M;

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

void Core::Components::BoxCollider::Update()
{
	//Application.GetEditorCamera()->UnProject(EditorUi::Editor::GetSceneWindow()->GetMousePosition()).Print();
	//Physic::Ray ray;
	//ray.Set(Application.GetEditorCamera()->Transform.GetLocalPosition(), Application.GetEditorCamera()->Transform.GetForwardVector() * 10000);
	//if (RayIntersection(ray))
		//printf("Collision");

}

void Core::Components::BoxCollider::GameUpdate()
{
	if (_dynamicBody) {
		auto pos = Math::Vec3(_dynamicBody->getGlobalPose().p.x, _dynamicBody->getGlobalPose().p.y, _dynamicBody->getGlobalPose().p.z);
		auto rot = Math::Quat(_dynamicBody->getGlobalPose().q.x, _dynamicBody->getGlobalPose().q.y, _dynamicBody->getGlobalPose().q.z, _dynamicBody->getGlobalPose().q.w);
		GameObject->Transform.SetLocalPosition(pos);
		GameObject->Transform.SetLocalRotation(rot);
	}
}

void Core::Components::BoxCollider::UpdateTransform()
{
}

void Core::Components::BoxCollider::ShowInInspector()
{
	ImGui::DragFloat3("Extent", &_extent[0], 0.1f);
}

void Core::Components::BoxCollider::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/BoxColliderIcon.png");
}

void Core::Components::BoxCollider::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Extent : %s\n", _extent.ToString().c_str());
}

void Core::Components::BoxCollider::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	while (currentLine.substr(0, 13) != "#EndComponent")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 6) == "Extent")
		{
			_extent = Utils::Loader::GetVector3(currentLine);
		}
		pos++;
	}
}


