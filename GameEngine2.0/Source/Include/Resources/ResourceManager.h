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
			auto res = std::shared_ptr<T>(new T());
			res->Load(filename);
			_resource[filename] = res;
			return res.get();
		}

		template<typename T> T* Get(const char* filename)
		{
			return dynamic_cast<T*>(_resource[filename]);
		}
		
	private:
		std::unordered_map<std::string, std::shared_ptr<Resources::IResource>> _resource;

		
	};
}