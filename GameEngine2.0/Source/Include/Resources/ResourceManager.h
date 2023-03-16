#pragma once
#include <unordered_map>
#include <memory>
#include <ImGui/imgui_internal.h>

#include "IResource.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "PhysicMaterial.h"

#include "Include/Utils/Utils.h"
namespace Resources {
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		template<typename T> T* Create(std::string filename, bool load = false)
		{
			if (_deleted || !std::filesystem::exists(filename))
				return nullptr;
			if (_resource.find(filename) != _resource.end())
				return dynamic_cast<T*>(_resource[filename]);

			std::string Path = filename;
			filename = filename.substr(filename.find_last_of("//\\") + 1);
			_resource[Path] = new T();
			_resource[Path]->SetPath(Path);
			_resource[Path]->SetName(filename);
			_resource[Path]->Load();
			return dynamic_cast<T*>(_resource[Path]);
		}

		template<typename T> T* Get(const char* filename)
		{
			if (!_deleted && _resource.count(filename))
				return dynamic_cast<T*>(_resource.at(filename));
			else
				return nullptr;
		}

		// Basicaly _resource[filename] = res (Replace the last one if exist);
		template<typename T> void Add(std::string filename, T* res)
		{
			if (_deleted)
			{
				delete res;
				return;
			}
			if (_resource.find(filename) == _resource.end()) {}
			else
			{
				// Delete if already exist to replace.
				delete _resource[filename];
			}
			_resource[filename] = res;
		}

		// Create A new Resource with the name as key (Replace the last one if exist).
		template<typename T> T* CreateNew(std::string filename)
		{
			if (_deleted)
				return nullptr;
			if (_resource.find(filename) == _resource.end()) {}
			else
			{
				// Delete if already exist to replace.
				delete _resource[filename];
			}
			_resource[filename] = new T();
			return dynamic_cast<T*>(_resource[filename]);
		}

		// Change the Key of an existing resource.
		template<typename T> void ChangeKey(std::string lastName, std::string newName, T* res)
		{
			_resource.erase(lastName);
			_resource[newName] = res;
		}

		template<typename T> T* ResourcesPopup(const char* popupName)
		{
			T* out = nullptr;
			int id = 0;
			if (ImGui::BeginPopupModal(popupName, (bool*)0, ImGuiWindowFlags_AlwaysAutoResize)) {
				static ImGuiTextFilter filter;
				filter.Draw();
				for (auto resource : _resource)
				{
					if (auto res = dynamic_cast<T*>(resource.second))
					{
						if (!filter.PassFilter(res->GetName().c_str()))
							continue;
						ImGui::PushID(id++);
						if (ImGui::Selectable(res->GetName().c_str(), false, ImGuiSelectableFlags_SpanAvailWidth)) {
							out = res;
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::IsItemHovered())
						{
							ImGui::BeginTooltip();
							ImGui::TextUnformatted(res->GetPath().c_str());
							ImGui::EndTooltip();
						}
						ImGui::PopID();
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
		Resources::Texture* TexturePopup(const char* popupName);

		void RemoveResource(Resources::IResource*);

		void LoadDefaultsResources();

		void RecompileShaders();

		PhysicMaterial* GetDefaultPhysicMaterial();
		Shader* GetDefaultShader();
		Shader* GetDefaultAnimShader();
		Shader* GetPickingShader();
		Shader* GetPickingAnimShader();
		Shader* GetBillboardShader();
		Shader* GetBillboardPickingShader();
		Mesh* GetDefaultCube();
		Mesh* GetDefaultPlane();

		std::unordered_map<std::string, Resources::IResource*> GetAllResources() { return _resource; }
		std::unordered_map<std::string, Resources::IResource*>* GetAllResourcesPtr() { return &_resource; }
		int TextureIndex = 1;

		bool IsDeleted() { return _deleted; }
	private:
		std::unordered_map<std::string, Resources::IResource*> _resource;
		bool _deleted = false;


	};
}