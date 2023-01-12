#pragma once
#include <Windows.h>
#include <glad/glad.h>
#include "Component.h"
namespace Resources
{
	class MeshInstance;
	class Shader;
	class Material;
}
namespace Render
{
	class InstanceManager;
	class EditorIcon;
}
namespace Core::Components
{
	class ParticleSystem;
	class Particle
	{
	public:
		Particle(ParticleSystem*, int index);
		~Particle();

		bool operator<(const Particle& that) const {
			// Sort in reverse order : far particles drawn first.
			return this->_alive > that._alive;
		}

		void Initialize();
		void ResetPosition();

		void Update();

		void SetIndex(size_t i) { _index = i; }

		Math::Vector4 GetXYZS() {return { _position, _alive ? _size : 0.0f };}
		size_t GetIndex() { return _index; }
		bool IsAlive() { return _alive; }
	private:
		size_t _index = -1;
		Math::Matrix4 _worldMat = Math::Matrix4::Identity();
		Math::Matrix4 _localMat = Math::Matrix4::Identity();
		ParticleSystem* _particleSystem;

		float _size = 0.25f;
		float _life = -1;
		float _startTime = 0.f;
		bool _alive = false;
		Math::Vector3 _position = Math::Vector3(0), _speed;
	};

	class ParticleSystem : public BaseComponent<ParticleSystem>
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		// Call When the size is changed
		void PostInitialize();
		void ResetPositions();
		void InitializeParticles();

		void Initialize() override;
		void Update() override;
		void DrawPicking(int index) override;
		void ShowInInspector() override;

		void SetSize(size_t size);
		void SetUIIcon() override;

		float GetTime() { return _timeSinceStart; }
		float GetLifeTime() { return _particlesLifeTime; }
		float GetAngle() { return _angle; }
		size_t GetMaxParticles() { return _maxParticles; }
		Math::Vector3 GetDirection() { return _mainDirection; }
		Math::Vector2 GetMinMaxSize() { return _size; }

		void Save(std::string space, std::string& content) override;
		void Load(const char* data, uint32_t& pos) override;

	private:
		Render::EditorIcon* _icon = nullptr;

		size_t _maxParticles = 100;
		Resources::Shader* _shader = nullptr;;
		std::vector<Particle*> _particles;
		std::vector<Math::Vector4>  XYZS;
		GLuint _buffer;
		Resources::MeshInstance* _mesh = nullptr;

		float _timeSinceStart = 0.f;
		float _speed = 1.f;
		bool _updateParticles = true;
		bool _drawParticles = true;
		float _particlesLifeTime = 5.f;
		float _angle = 25.f;
		Math::Vector3 _mainDirection = Math::Vector3(0.0f, 10.0f, 0.0f);
		Math::Vector2 _size = Math::Vector2(0.1f , 0.2f);

	};
}