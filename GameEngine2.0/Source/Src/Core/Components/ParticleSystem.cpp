#include "Include/Core/Components/ParticleSystem.h"
#include "Include/Render/InstancesManager.h"
#include "Include/Render/EditorIcon.h"
#include "Include/Utils/Utils.h"
#include "Include/Utils/Loader.h"
#include "Include/Resources/MeshInstance.h"
#include "Include/Resources/Shader.h"
#include "Include/App.h"
#include "Include/Core/Node.h"
#include "Include/Core/Transform.h"

#include <Windows.h>
#include <glad/glad.h>
#include <algorithm>

#pragma region ParticleSystem
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
	delete _mesh;
}

void Core::Components::ParticleSystem::Initialize()
{
	_mesh = new Resources::MeshInstance(*(Resources::MeshInstance*)Application.GetResourceManager()->Get<Resources::Mesh>("DefaultPlane"));
	this->_shader = Application.GetResourceManager()->Get<Resources::Shader>("Assets/Default/Shaders/BillboardInstanceShader");
	SetSize(_maxParticles);
}

void Core::Components::ParticleSystem::PostInitialize()
{
	if (_particles.size() == 0)
		return;
	if (_buffer > 0)
		glDeleteBuffers(1, &_buffer);
	glGenBuffers(1, &_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	std::vector<Math::Vector4>  XYZS;
	XYZS.resize(_particles.size());
	glBufferData(GL_ARRAY_BUFFER, XYZS.size() * sizeof(float[4]), &XYZS[0], GL_STREAM_DRAW);

	InitializeParticles();
}

void Core::Components::ParticleSystem::ResetPositions()
{
	for (auto p : _particles)
	{
		p->ResetPosition();
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
	_icon->Draw(Application.GetScene()->GetVPMatrix(), GameObject->Transform, GameObject->IsSelected());

	if (_updateParticles) {
		if (XYZS.size() != _particles.size())
			XYZS.resize(_particles.size());
		for (int i = 0; i < _particles.size(); i++)
		{
			_particles[i]->Update();
			XYZS[i] = (_particles[i]->GetXYZS());
		}
		if (_particles.size() > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, XYZS.size() * 4 * sizeof(float), &XYZS[0]);
		}
		_timeSinceStart += ImGui::GetIO().DeltaTime * _speed;
	}

	// Sent Values to Shader
	if (!_shader || _particles.size() == 0 || !_mesh || !_drawParticles)
		return;
	glUseProgram(_shader->Program);
	auto up = Application.GetScene()->GetCameraEditor()->Transform.GetUpVector();
	auto right = Application.GetScene()->GetCameraEditor()->Transform.GetRightVector();
	glUniform1i(_shader->GetLocation(Resources::Location::L_ENABLE_TEXTURE), _mesh->SubMeshes[0].Material->GetTexture() ? true : false);
	if (_mesh->SubMeshes[0].Material->GetTexture())
		glUniform1i(_shader->GetLocation(Resources::Location::L_TEXTURE), _mesh->SubMeshes[0].Material->GetTexture()->GetIndex());
	else
		glUniform4f(_shader->GetLocation(Resources::Location::L_COLOR), _mesh->SubMeshes[0].Material->GetDiffuse().x, _mesh->SubMeshes[0].Material->GetDiffuse().y, _mesh->SubMeshes[0].Material->GetDiffuse().z, _mesh->SubMeshes[0].Material->GetDiffuse().w);

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
	if (ImGui::Begin("Particles", (bool*)true, ImGuiWindowFlags_NoDocking))
	{
		if (ImGui::Button(_updateParticles ? "Pause" : "Play")) { _updateParticles = !_updateParticles; }
		ImGui::SameLine();
		if (ImGui::Button("Restart"))
		{
			_timeSinceStart = 0;
			ResetPositions();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop")) {
			_timeSinceStart = 0;
			_updateParticles = false;
			_drawParticles = false;
			ResetPositions();
		}
		ImGui::PushItemWidth(100.f);
		ImGui::DragFloat("Playback Time", &_timeSinceStart);
		ImGui::DragFloat("Playback Speed", &_speed);
		if (_speed < 0) _speed = 0;
		ImGui::PopItemWidth();
	}
	ImGui::End();
	int size = (int)this->_maxParticles;
	if (ImGui::InputInt("Particles Count", &size, 1, 100, ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (size != this->_maxParticles && size >= 0)
		{
			this->SetSize(size);
		}
	}
	if (ImGui::DragFloat("Life Time", &_particlesLifeTime))
	{
		InitializeParticles();
	}
	ImGui::DragFloat3("Direction", &_mainDirection.x, 0.1f);
	ImGui::DragFloatRange2("Min/Max Size", &_size.x, &_size.y, 0.01f, 0.0f, 1.0f, "%.5f");
	ImGui::DragFloat("Angle", &_angle, 1.0f, 0.f, 360.f);
	ImGui::Checkbox("Draw Particles", &_drawParticles);

	// Material
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
	_mesh = new Resources::MeshInstance(*static_cast<Resources::MeshInstance*>(mesh));
	for (auto p : _particles)
	{
		p->SetMesh(_mesh);
	}
}
void Core::Components::ParticleSystem::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("MaxParticle : %d\n", _maxParticles);
	content += space + Utils::StringFormat("MainDirection : %s\n", _mainDirection.ToString().c_str());
	content += space + Utils::StringFormat("LifeTime : %f\n", _particlesLifeTime);
	content += space + Utils::StringFormat("Size : %s\n", _size.ToString().c_str());
	content += space + Utils::StringFormat("Angle : %f\n", _angle);
}

void Core::Components::ParticleSystem::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	while (currentLine.substr(0, 13) != "#EndComponent")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 11) == "MaxParticle")
		{
			int max = Utils::Loader::GetInt(currentLine);
			_maxParticles = max;
		}
		else if (currentLine.substr(0, 13) == "MainDirection")
		{
			auto dir = Utils::Loader::GetVector3(currentLine);
			_mainDirection = dir;
		}
		else if (currentLine.substr(0, 13) == "LifeTime")
		{
			auto life = Utils::Loader::GetFloat(currentLine);
			_particlesLifeTime = life;
		}
		else if (currentLine.substr(0, 4) == "Size")
		{
			auto size = Utils::Loader::GetVector2(currentLine);
			_size = size;
		}
		else if (currentLine.substr(0, 5) == "Angle")
		{
			auto angle = Utils::Loader::GetFloat(currentLine);
			_angle = angle;
		}
		pos++;
	}

}

void Core::Components::ParticleSystem::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/ParticleIcon.png");
}

void Core::Components::ParticleSystem::InitializeParticles()
{
	for (auto instance : _particles)
	{
		instance->Initialize();
	}
}

#pragma endregion

#pragma region Particle
Core::Components::Particle::Particle(ParticleSystem* ps, int index)
{
	_index = index;
	_particleSystem = ps;
}

Core::Components::Particle::~Particle()
{
}

void Core::Components::Particle::Initialize()
{
	_mesh->Initialize();
	ResetPosition();
	_startTime = (float)(_index * _particleSystem->GetLifeTime()) / (float)_particleSystem->GetMaxParticles();
}

void Core::Components::Particle::ResetPosition()
{
	_startTime = 0.f;
	float spread = 1.5f;
	_position = _particleSystem->GameObject->Transform.GetWorldPosition();
	float angle = (_particleSystem->GetAngle() * 10.f) / 360.f;
	Math::Vector3 randomdir = Math::Vector3(
		Utils::RandomFloat(-angle, angle),
		Utils::RandomFloat(-angle, angle),
		Utils::RandomFloat(-angle, angle)
	);
	_alive = false;
	_speed = _particleSystem->GetDirection() + randomdir * 1.5f;
	_life = 0.f;
	auto size = _particleSystem->GetMinMaxSize();
	_size = Utils::RandomFloat(size.x, size.y);
}

void Core::Components::Particle::Update()
{
	if (_life > (_startTime + _particleSystem->GetLifeTime()))
	{
		ResetPosition();
	}
	else if (_life >= _startTime)
	{
		_alive = true;
		_speed += Math::Vector3(0.0f, -9.81f, 0.0f) * (float)ImGui::GetIO().DeltaTime * 0.5f;
		_position += _speed * (float)ImGui::GetIO().DeltaTime;
	}
	_life += ImGui::GetIO().DeltaTime;
}

void Core::Components::Particle::Draw(Resources::Shader* shader, int amount)
{
	_mesh->Draw(shader, amount);
}

void Core::Components::Particle::SetMesh(Resources::MeshInstance* mesh)
{
	if (!mesh)
		return;
	_mesh = mesh;
}

void Core::Components::Particle::SetMaterial(Resources::Material* mat)
{
	_mesh->SubMeshes[0].Material = mat;
}
#pragma endregion