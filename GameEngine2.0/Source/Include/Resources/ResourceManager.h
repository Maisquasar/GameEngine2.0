#pragma once
#include <unordered_map>
#include <memory>

#include "IResource.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"

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
			_resource[Path] = new T();
			_resource[Path]->SetPath(Path);
			_resource[Path]->SetName(filename);
			_resource[Path]->Load(filename);
			return dynamic_cast<T*>(_resource[Path]);
		}

		template<typename T> static T* Get(const char* filename)
		{
			if (_resource.count(filename))
				return dynamic_cast<T*>(_resource.at(filename));
			else
				return nullptr;
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

		// Basicaly _resource[filename] = res;
		template<typename T> static void Add(std::string filename, T* res)
		{
			_resource[filename] = res;
		}

		template<typename T> static void ChangeKey(std::string lastName, std::string newName, T* res)
		{
			_resource.erase(lastName);
			_resource[newName] = res;
		}

		void RecompileShaders();

		static Shader* GetDefaultShader();
		static Shader* GetPickingShader();
		static Mesh* GetDefaultCube();

		template<typename T> static T* ResourcesPopup(const char* popupName)
		{
			T* out = nullptr;
			if (ImGui::BeginPopupModal(popupName, (bool*)0, ImGuiWindowFlags_AlwaysAutoResize)) {
				for (auto resource : _resource)
				{
					if (auto res = dynamic_cast<T*>(resource.second))
					{
						if (ImGui::Button(res->GetName().c_str())) {
							out = res;
							ImGui::CloseCurrentPopup();
						}
					}
				}
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			return out;
		}
		static std::unordered_map<std::string, Resources::IResource*> GetAllResources() { return _resource; }
		static int TextureIndex;

	private:
		static std::unordered_map<std::string, Resources::IResource*> _resource;

		
	};
}