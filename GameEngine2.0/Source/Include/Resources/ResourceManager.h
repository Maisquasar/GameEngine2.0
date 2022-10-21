#pragma once
#include <unordered_map>
#include <memory>

#include "IResource.h"
#include "Texture.h"

namespace Resources {
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		template<typename T> T* Create(const char* filename)
		{
			_resource[filename] = new T();
			_resource[filename]->Load(filename);
			return dynamic_cast<T*>(_resource[filename]);
		}

		template<typename T> static T* Get(const char* filename)
		{
			return dynamic_cast<T*>(_resource[filename]);
		}
		
	private:
		static std::unordered_map<std::string, Resources::IResource*> _resource;

		
	};
}