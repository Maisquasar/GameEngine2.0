#include "Include/Render/InstancesManager.h"
#include "Include/Resources/MeshInstance.h"
#include "Include/Resources/Shader.h"
#include "Include//App.h"

Render::InstancesManager::InstancesManager()
{
}

Render::InstancesManager::~InstancesManager()
{
	for (auto instance : _instances)
	{
		delete instance;
	}
}

void Render::InstancesManager::Initialize()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	std::vector<Math::Matrix4>  modelMatrices;
	int i = 0;
	for (auto instance : _instances)
	{
		modelMatrices.push_back(Math::Matrix4::CreateTransformMatrix(Math::Vector3(i), Math::Vector3(0), Math::Vector3(1)).TransposeMatrix());
		i++;
	}
	glBufferData(GL_ARRAY_BUFFER, _instances.size() * sizeof(float[16]), &modelMatrices[0].content[0][0], GL_STATIC_DRAW);

	for (auto instance : _instances)
	{
		instance->Initialize();
	}
}

void Render::InstancesManager::Draw()
{
	if (!_currentShader)
		return;
	glUseProgram(_currentShader->Program);
	glUniformMatrix4fv(_currentShader->GetLocation(Resources::Location::L_VIEWMATRIX), 1, GL_TRUE, &Application.GetScene()->GetCameraEditor()->GetViewMatrix().content[0][0]);
	glUniformMatrix4fv(_currentShader->GetLocation(Resources::Location::L_PROJECTIONMATRIX), 1, GL_TRUE, &Application.GetScene()->GetCameraEditor()->GetProjection().content[0][0]);
	for (int i = 0; i < _instances.size(); i++)
	{
		_instances[i]->Draw(this->_currentShader, (int)_instances.size());
	}
}

void Render::InstancesManager::SetInstances(Resources::MeshInstance* ref, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		auto mesh = new Resources::MeshInstance(*static_cast<Resources::MeshInstance*>(ref));
		_instances.push_back(mesh);
	}
}
