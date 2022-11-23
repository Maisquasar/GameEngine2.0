#pragma once
#include <unordered_map>
#include <memory>

#include "IResource.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"

namespace Core::Components
{
	class Mesh;
}
namespace Resources {
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		template<typename T> static T* Create(std::string filename)
		{
			std::string Path = filename;
			filename = filename.substr(filename.find_last_of("//\\") + 1);
			_resource[filename] = new T();
			_resource[filename]->SetPath(Path);
			_resource[filename]->SetName(filename);
			_resource[filename]->Load(filename);
			return dynamic_cast<T*>(_resource[filename]);
		}

		template<typename T> static T* Get(const char* filename)
		{
			return dynamic_cast<T*>(_resource.at(filename));
		}

		template<typename T> static T* GetWithPath(const char* filename)
		{
			for (auto r : _resource)
			{
				if (r.second->GetPath() == filename)
					return dynamic_cast<T*>(r.second);
			}
			return nullptr;
		}

		template<typename T> static void Add(std::string filename, T* res)
		{
			_resource[filename] = res;
		}

		static Resources::Mesh* MeshPopup();
		
	private:
		static std::unordered_map<std::string, Resources::IResource*> _resource;

		
	};
}