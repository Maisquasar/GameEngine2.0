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
			_resource[filename] = std::make_unique<T>();
			_resource[filename]->Load(filename);
			return dynamic_cast<T*>(_resource[filename].get());
		}

		template<typename T> static T* Get(const char* filename)
		{
			return dynamic_cast<T*>(_resource[filename].get());
		}
		
	private:
		static std::unordered_map<std::string, std::unique_ptr<Resources::IResource>> _resource;

		
	};
}