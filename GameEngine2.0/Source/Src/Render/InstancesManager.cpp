#include "Include/Render/InstancesManager.h"
#include "Include/Resources/MeshInstance.h"
#include "Include/Resources/Shader.h"
#include "Include//App.h"

Render::InstancesManager::InstancesManager()
{
}

Render::InstancesManager::~InstancesManager()
{
	if (_instances[0])
		delete _instances[0];
}

void Render::InstancesManager::Initialize()
{
	glGenBuffers(1, &_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	std::vector<Math::Mat4>  modelMatrices;
	int i = 0;
	int j = -1;
	int modulo = 100;
	for (auto instance : _instances)
	{
		if (i % modulo == 0)
			j++;

		modelMatrices.push_back(Math::Mat4::CreateTransformMatrix(Math::Vec3((float)(i % modulo) - modulo / 2, (float)j*2, 0) * 2, Math::Vec3(0), Math::Vec3(1)).TransposeMatrix());
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
	static float i = 0;
	if (!_currentShader)
		return;
	glUseProgram(_currentShader->Program);
	glUniformMatrix4fv(_currentShader->GetLocation(Resources::Location::L_VIEWMATRIX), 1, GL_TRUE, &Application.GetScene()->GetCameraEditor()->GetViewMatrix().content[0][0]);
	glUniformMatrix4fv(_currentShader->GetLocation(Resources::Location::L_PROJECTIONMATRIX), 1, GL_TRUE, &Application.GetScene()->GetCameraEditor()->GetProjection().content[0][0]);

	//glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	//auto mat = Math::Mat4::CreateTransformMatrix(Math::Vec3(0.0f), Math::Vec3(i, 0, 0), Math::Vec3(1));
	//glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), &mat.content[0][0]);
	_instances[0]->Draw((int)_instances.size());

	i += ImGui::GetIO().DeltaTime * 10;
}

void Render::InstancesManager::SetInstances(Resources::MeshInstance* ref, size_t size)
{
	auto mesh = new Resources::MeshInstance(*static_cast<Resources::MeshInstance*>(ref));
	for (int i = 0; i < size; i++)
	{
		_instances.push_back(mesh);
	}
}
