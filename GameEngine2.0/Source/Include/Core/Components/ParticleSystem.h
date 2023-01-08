#pragma once
#include <Windows.h>
#include <glad/glad.h>
#include "Component.h"
namespace Resources
{
	class MeshInstance;
}
namespace Render
{
	class InstanceManager;
}
namespace Core::Components
{
	class Particle
	{
	public:
		Particle();
		~Particle();

		void Initialize();

		void Update();

	private:

	};

	class ParticleSystem : public BaseComponent<ParticleSystem>
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		void Initialize() override;

		size_t MaxParticles;

	private:
		std::vector<Particle*> _particles;
		GLuint _billboardVertexBuffer;
		GLuint _particlesPositionBuffer;
		GLuint _particlesColorBuffer;

	};
}