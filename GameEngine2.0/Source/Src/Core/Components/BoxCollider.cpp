#include "Include/Core/Components/BoxCollider.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include "Include/Math/Math.h"
#include "Include/Physic/Physic.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/Physic/Ray.h"
#include "Include/Debug/Line.h"
#include "Include/Core/Transform.h"


Core::Components::BoxCollider::BoxCollider()
{
	ComponentName = "BoxCollider";
	Initialize();
}

Core::Components::BoxCollider::BoxCollider(Math::Vec3 Position, Math::Vec3 Size, Math::Quat Rotation)
{
	ComponentName = "BoxCollider";
	Transform.SetLocalPosition(Position);
	Transform.SetLocalRotation(Rotation);
	Transform.SetLocalScale(Size);
	Initialize();
}

void Core::Components::BoxCollider::SetGameobject(Core::Node* node)
{
	Super::SetGameobject(node);
	Transform.Parent = node;
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
}

void Core::Components::BoxCollider::Update()
{
	Transform.Update();
	glUseProgram(_shader->Program);
	// Set the Model Matrix.
	Math::Mat4 M = Transform.GetModelMatrix();
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
	glDraw(GL_TRIANGLES, 0, (GLsizei)_vertices.size());

	// Disable Wire frame.
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	//Application.GetEditorCamera()->UnProject(Application.GetFramebuffer()->GetMousePosition()).Print();
	//Physic::Ray ray;
	//ray.Set(Application.GetEditorCamera()->Transform.GetLocalPosition(), Application.GetEditorCamera()->Transform.GetForwardVector() * 10000);
	//if (RayIntersection(ray))
		//printf("Collision");

}

void Core::Components::BoxCollider::UpdateTransform()
{
	Transform.ForceUpdate();
}

void Core::Components::BoxCollider::ShowInInspector()
{
	Transform.ShowInInspector();
}

void Core::Components::BoxCollider::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/BoxColliderIcon.png");
}

bool Core::Components::BoxCollider::RayIntersection(Physic::Ray ray)
{
	//Math::Vec3 local_origin = glm::vec3(glm::inverse(box_transform) * glm::vec4(origin, 1));
	//Math::Vec3 local_direction = glm::vec3(glm::inverse(box_transform) * glm::vec4(direction, 0));
	auto min = Transform.GetWorldRotation() * Math::Vec3(-Transform.GetWorldScale());
	auto max = Transform.GetWorldRotation() * Math::Vec3(Transform.GetWorldScale());
	float tmin = (min.x - ray.GetOrigin().x) / ray.GetDirection().x;
	float tmax = (max.x - ray.GetOrigin().x) / ray.GetDirection().x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (min.y - ray.GetOrigin().y) / ray.GetDirection().y;
	float tymax = (max.y - ray.GetOrigin().y) / ray.GetDirection().y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.z - ray.GetOrigin().z) / ray.GetDirection().z;
	float tzmax = (max.z - ray.GetOrigin().z) / ray.GetDirection().z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;
	return true;
	
}


