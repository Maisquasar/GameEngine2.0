#include "..\..\Include\Resources\Model.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/Core/Components/MeshComponent.h"

Resources::Model::Model()
{
}

Resources::Model::~Model()
{
}

void Resources::Model::Load(std::string filename)
{
#if MULTITHREAD_LOADING
	Application.ThreadManager.QueueJob(&Model::MultiThreadLoad, this, filename);
#else
	MultiThreadLoad(filename);
#endif
}

void Resources::Model::LoadSingleThread(std::string filename)
{
	MultiThreadLoad(filename);
}

void Resources::Model::MultiThreadLoad(std::string filename)
{
	uint32_t size = 0;
	bool sucess = false;
	LOG(Debug::LogType::INFO, "Loading Model : %s", _path.c_str());
	auto data = Utils::Loader::ReadFile(_path.c_str(), size, sucess);
	if (sucess)
		ModelLoop(data, size);
	else
		PrintError("Failed loaded Model : %s", _path.c_str());
	LOG(Debug::LogType::INFO, "Successfully loaded Model : %s", _path.c_str());
	delete[] data;
}

Core::Node* Resources::Model::CreateFromModel()
{
	auto node = new Core::Node(*this);
	node->Childrens.clear();
	for (int i = 0; i < Childrens.size(); i++)
	{
		auto newChild = new Core::Node(*Childrens[i]);
		newChild->Components.clear();
		for (auto comp : newChild->Components)
		{
			newChild->AddComponent(comp->Clone());
		}
		node->AddChildren(newChild);
	}
	return node;
}

bool Resources::Model::IsInitialized()
{
	/*
	for (auto child : Childrens)
	{
		if (auto mesh = dynamic_cast<Core::Components::MeshComponent*>(child->Components[0])->GetMesh())
		{
			if (!mesh->IsInitialized())
				return false;
		}
	}
	*/
	return _initialized;
}


void Resources::Model::ModelLoop(const char* data, const int32_t& size)
{
	uint32_t pos = 0;
	std::string currentLine;
	std::string prefix;
	std::vector<Resources::Mesh> Meshes;
	Math::Vec3 lastSize;
	this->Name = GetName();
	while (pos != size)
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		prefix = currentLine.substr(0, 2);
		if (prefix == "o ")
		{
			if (Meshes.size() > 0) {
				lastSize = lastSize + Math::Vec3((float)Meshes.back().Positions.size(), (float)Meshes.back().TextureUVs.size(), (float)Meshes.back().Normals.size());

				if (Meshes.back().SubMeshes.size() > 0)
					Meshes.back().SubMeshes.back().Count = Meshes.back().Indices.size() - Meshes.back().SubMeshes.back().StartIndex;
				Meshes.back().Load("");
				Meshes.back().SetPath(GetPath() + "::" + Meshes.back().GetName());
				auto meshptr = new Resources::Mesh(Meshes.back());
				Application.GetResourceManager()->Add(GetPath() + "::" + Meshes.back().GetName(), meshptr);
				this->Childrens.back()->GetComponent<Core::Components::MeshComponent>()->SetMesh(meshptr);
				this->Childrens.back()->Name = meshptr->GetName().c_str();
			}
			Meshes.push_back(Resources::Mesh());
			std::string Name = Utils::Loader::GetString(currentLine);
			Meshes.back().SetName(Name);
			this->AddChildren(new Node());
			this->Childrens.back()->AddComponent(new Core::Components::MeshComponent());
		}
		else if (prefix == "mt")
		{
			std::string path = currentLine;
			path = path.substr(path.find_first_of(' ') + 1);
			path = _path.substr(0, _path.find_last_of('/') + 1) + path;
			Utils::Loader::MtlLoader(path);
		}
		else if (prefix == "v ")
		{
			Meshes.back().Positions.push_back(Utils::Loader::GetVec3(currentLine));
		}
		else if (prefix == "vn")
		{
			Meshes.back().Normals.push_back(Utils::Loader::GetVec3(currentLine));
		}
		else if (prefix == "vt")
		{
			Meshes.back().TextureUVs.push_back(Utils::Loader::GetVec2(currentLine));
		}
		else if (prefix == "f ")
		{
			// If No use material
			if (Meshes.back().SubMeshes.size() == 0)
			{
				Meshes.back().SubMeshes.push_back(SubMesh());
				//Meshes.back().SubMeshes.back().Material = Application.GetResourceManager()->Get<Resources::Material>("DefaultMaterial");
			}
			auto indices = Utils::Loader::GetIndices(currentLine);
			for (size_t i = 0; i < 3; i++)
			{
				indices[i] = indices[i] - lastSize;
				Meshes.back().Indices.push_back(indices[i]);
			}
		}
		else if (prefix == "us")
		{
			// Set SubMesh
			if (Meshes.back().SubMeshes.size() > 0)
				Meshes.back().SubMeshes.back().Count = Meshes.back().Indices.size() - Meshes.back().SubMeshes.back().StartIndex;
			auto subMesh = SubMesh();
			subMesh.StartIndex = Meshes.back().Indices.size();

			// Set Name
			std::string MaterialName = Utils::Loader::GetString(currentLine);
			MaterialName = GetPath().substr(0, GetPath().find_last_of('/') + 1) + MaterialName + ".mat";
			Resources::Material* mat;
			if (mat = Application.GetResourceManager()->Get<Resources::Material>(MaterialName.c_str())) {
				this->Childrens.back()->GetComponent<Core::Components::MeshComponent>()->AddMaterial(mat);
			}
			else
			{
				if (mat = Application.GetResourceManager()->Create<Resources::Material>(MaterialName))
				{

				}
				else
					mat = Application.GetResourceManager()->Get<Resources::Material>("DefaultMaterial");
				this->Childrens.back()->GetComponent<Core::Components::MeshComponent>()->AddMaterial(mat);
			}
			Meshes.back().SubMeshes.push_back(subMesh);
		}
		pos++;
	}
	if (Meshes.back().SubMeshes.size() > 0) {
		Meshes.back().SubMeshes.back().Count = Meshes.back().Indices.size() - Meshes.back().SubMeshes.back().StartIndex;
		Meshes.back().Load("");
		Meshes.back().SetPath(GetPath() + "::" + Meshes.back().GetName());
		auto meshptr = new Resources::Mesh(Meshes.back());
		Application.GetResourceManager()->Add(GetPath() + "::" + Meshes.back().GetName(), meshptr);
		this->Childrens.back()->GetComponent<Core::Components::MeshComponent>()->SetMesh(meshptr);
		this->Childrens.back()->Name = meshptr->GetName().c_str();
	}
	_initialized = true;
}