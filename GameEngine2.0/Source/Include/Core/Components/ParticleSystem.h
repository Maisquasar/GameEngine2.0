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
}
namespace Core::Components
{
	class ParticleSystem;
	class Particle
	{
	public:
		Particle(ParticleSystem* , int index);
		~Particle();

		void Initialize();
		void ResetPosition();

		void Update();
		void Draw(Resources::Shader* shader, int amount);

		void SetMesh(Resources::MeshInstance* mesh);
		void SetMaterial(Resources::Material* mat);
	private:
		Resources::MeshInstance* _mesh = nullptr;
		size_t _index = -1;
		Math::Matrix4 _worldMat = Math::Matrix4::Identity();
		Math::Matrix4 _localMat = Math::Matrix4::Identity();
		ParticleSystem* _particleSystem;

		float _life = -1;
		float _startTime = 0.f;
		Math::Vector3 _position = Math::Vector3(0), _speed;
	};

	class ParticleSystem : public BaseComponent<ParticleSystem>
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void Initialize() override;
		// Call When the size is changed
		void PostInitialize();

		void Update() override;

		void ShowInInspector() override;

		void SetSize(size_t size);
		void SetMesh(Resources::MeshInstance* mesh);

		float GetLifeTime() { return _particlesLifeTime; }
		size_t GetMaxParticles() { return _maxParticles; }
		Math::Vector3 GetDirection() { return _mainDirection; }

	private:
		size_t _maxParticles = 100;
		Resources::Shader* _shader = nullptr;;
		std::vector<Particle*> _particles;
		GLuint _buffer;
		Resources::MeshInstance* _mesh = nullptr;

		bool _updateParticles = true;
		float _particlesLifeTime = 5.f;
		Math::Vector2 _particlesSize = Math::Vector2(0.1f, 0.1f);
		Math::Vector3 _mainDirection = Math::Vector3(0.0f, 10.0f, 0.0f);

	};
}