#include "Include/Core/Components/ParticleSystem.h"
#include "Include/Render/InstancesManager.h"
#include "Include/Render/EditorIcon.h"
#include "Include/Resources/MeshInstance.h"
#include "Include/Resources/Shader.h"
#include "Include/App.h"
#include "Include/Core/Node.h"
#include "Include/Core/Transform.h"

#include <Windows.h>
#include <glad/glad.h>

Core::Components::ParticleSystem::ParticleSystem()
{
	ComponentName = "Particle System";
}

Core::Components::ParticleSystem::~ParticleSystem()
{
	for (auto p : _particles)
	{
		delete p;
	}
	_particles.clear();
	delete _icon;
}

void Core::Components::ParticleSystem::Initialize()
{
	_mesh = ((Resources::MeshInstance*)Application.GetResourceManager()->Get<Resources::Mesh>("DefaultPlane"));
	this->_shader = Application.GetResourceManager()->Get<Resources::Shader>("Assets/Default/Shaders/BillboardInstanceShader");
	SetSize(_maxParticles);
}

void Core::Components::ParticleSystem::PostInitialize()
{
	if (_buffer > 0)
		glDeleteBuffers(1, &_buffer);
	glGenBuffers(1, &_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	std::vector<Math::Matrix4>  modelMatrices;
	for (auto instance : _particles)
	{
		modelMatrices.push_back(Math::Matrix4::Identity());
	}
	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(float[16]), &modelMatrices[0].content[0][0], GL_STREAM_DRAW);

	for (auto instance : _particles)
	{
		instance->Initialize();
	}
}

void Core::Components::ParticleSystem::Update()
{
	if (!_icon)
	{
		_icon = new Render::EditorIcon();
		_icon->Initialize("ParticleMat");
		_icon->SetSize(Math::Vector2(0.25f, 0.25f));

	}
	_icon->Draw(Application.GetScene()->GetVPMatrix(), GameObject->Transform);

	if (!_shader || _particles.size() == 0 || !_mesh)
		return;
	if (_updateParticles) {
		for (auto p : _particles)
		{
			p->Update();
		}
	}
	glUseProgram(_shader->Program);
	auto up = Application.GetScene()->GetCameraEditor()->Transform.GetUpVector();
	auto right = Application.GetScene()->GetCameraEditor()->Transform.GetRightVector();
	glUniform1i(_shader->GetLocation(Resources::Location::L_ENABLE_TEXTURE), _mesh->SubMeshes[0].Material->GetTexture() ? true : false);
	if (_mesh->SubMeshes[0].Material->GetTexture())
		glUniform1i(_shader->GetLocation(Resources::Location::L_TEXTURE), _mesh->SubMeshes[0].Material->GetTexture()->GetIndex());
	else
		glUniform4f(_shader->GetLocation(Resources::Location::L_COLOR), _mesh->SubMeshes[0].Material->GetDiffuse().x, _mesh->SubMeshes[0].Material->GetDiffuse().y, _mesh->SubMeshes[0].Material->GetDiffuse().z, _mesh->SubMeshes[0].Material->GetDiffuse().w);

	glUniform2f(_shader->GetLocation(Resources::Location::L_BILLSIZE), _particlesSize.x, _particlesSize.y);
	glUniform3f(_shader->GetLocation(Resources::Location::L_CAMUP), up.x, up.y, up.z);
	glUniform3f(_shader->GetLocation(Resources::Location::L_CAMRIGHT), right.x, right.y, right.z);
	auto vp = Application.GetScene()->GetCameraEditor()->GetProjection() * Application.GetScene()->GetCameraEditor()->GetViewMatrix();
	glUniformMatrix4fv(_shader->GetLocation(Resources::Location::L_VIEWPROJECTIONMATRIX), 1, GL_TRUE, &vp.content[0][0]);
	_particles[0]->Draw(this->_shader, (int)_particles.size());
}

void Core::Components::ParticleSystem::DrawPicking(int index)
{
	_icon->DrawPicking(Application.GetScene()->GetVPMatrix(), GameObject->Transform, index);
}

void Core::Components::ParticleSystem::ShowInInspector()
{
	int size = (int)this->_maxParticles;
	if (ImGui::InputInt("Particles Count", &size, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (size != this->_maxParticles && size >= 0)
		{
			this->SetSize(size);
		}
	}
	ImGui::Checkbox("Update", &_updateParticles);
	ImGui::DragFloat("Life Time", &_particlesLifeTime);
	ImGui::DragFloat3("Direction", &_mainDirection.x, 0.1f);
	ImGui::TextUnformatted(_mesh->SubMeshes[0].Material->GetPath().c_str());
	if (ImGui::Button("Change Material"))
	{
		ImGui::OpenPopup("MaterialPopup");
	}
	if (auto mat = Application.GetResourceManager()->ResourcesPopup<Resources::Material>("MaterialPopup"))
	{
		_mesh->SubMeshes[0].Material = mat;
		for (auto p : _particles)
		{
			p->SetMaterial(mat);
		}
	}
}

void Core::Components::ParticleSystem::SetSize(size_t size)
{
	_maxParticles = size;
	for (auto p : _particles)
	{
		delete p;
	}
	_particles.clear();
	for (int i = 0; i < _maxParticles; i++)
	{
		_particles.push_back(new Particle(this, i));
		_particles.back()->SetMesh(_mesh);
	}
	this->PostInitialize();
}

void Core::Components::ParticleSystem::SetMesh(Resources::MeshInstance* mesh)
{
	_mesh = mesh;
	for (auto p : _particles)
	{
		p->SetMesh(_mesh);
	}
}

Core::Components::Particle::Particle(ParticleSystem* ps, int index)
{
	_index = index;
	_particleSystem = ps;
}

Core::Components::Particle::~Particle()
{
	if (_mesh) {
		delete _mesh;
		_mesh = nullptr;
	}
}

void Core::Components::Particle::Initialize()
{
	_mesh->Initialize();
	ResetPosition();
}

void Core::Components::Particle::ResetPosition()
{
	float spread = 1.5f;
	_position = _particleSystem->GameObject->Transform.GetWorldPosition();
	Math::Vector3 randomdir = Math::Vector3(
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f,
		(rand() % 2000 - 1000.0f) / 1000.0f
	);
	_startTime = (float)(_index * 5.f) / (float)_particleSystem->GetMaxParticles();
	_speed = _particleSystem->GetDirection() + randomdir * 1.5f;
	_life = 0;
}

void Core::Components::Particle::Update()
{
	if (_life > _startTime + _particleSystem->GetLifeTime())
	{
		ResetPosition();
	}
	else if (_life >= _startTime)
	{
		_speed += Math::Vector3(0.0f, -9.81f, 0.0f) * (float)ImGui::GetIO().DeltaTime * 0.5f;
		_position += _speed * (float)ImGui::GetIO().DeltaTime;
	}
	_life += ImGui::GetIO().DeltaTime;

	_localMat = Math::Matrix4::CreateTranslationMatrix(_position, true);
	glBufferSubData(GL_ARRAY_BUFFER, _index * 16 * sizeof(float), 16 * sizeof(float), &_localMat.content[0][0]);
}

void Core::Components::Particle::Draw(Resources::Shader* shader, int amount)
{
	_mesh->Draw(shader, amount);
}

void Core::Components::Particle::SetMesh(Resources::MeshInstance* mesh)
{
	_mesh = new Resources::MeshInstance(*static_cast<Resources::MeshInstance*>(mesh));
}

void Core::Components::Particle::SetMaterial(Resources::Material* mat)
{
	_mesh->SubMeshes[0].Material = mat;
}
