#pragma once
#include <vector>
namespace Resources
{
	class MeshInstance;
	class Shader;
}

namespace Render
{
	class InstancesManager
	{
	public:
		InstancesManager();
		~InstancesManager();

		void Initialize();

		void Draw();

		void SetInstances(Resources::MeshInstance*, size_t size);

		Resources::MeshInstance* GetInstance(size_t index) { return _instances.at(index); }
		std::vector<Resources::MeshInstance*> GetInstances() { return _instances; }

		void SetShader(Resources::Shader* s) { _currentShader = s; }
	private:
		std::vector<Resources::MeshInstance*> _instances;
		Resources::Shader* _currentShader = nullptr;
		unsigned int _buffer;
	};
}