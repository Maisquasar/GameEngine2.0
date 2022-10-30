#include "..\..\Include\Resources\Model.h"
#include "Include/Resources/ResourceManager.h"

Resources::Model::Model()
{
}

Resources::Model::~Model()
{
}

void Resources::Model::Load(std::string filename)
{
	uint32_t size = 0;
	bool sucess = false;
	auto data = Utils::Loader::ReadFile(_path.c_str(), size, sucess);
	if (sucess)
		ModelLoop(data, size);
	LOG(Debug::LogType::INFO, "Successfully loaded Model : %s", _path.c_str());
	delete[] data;
}

void Resources::Model::ModelLoop(const char* data, const int32_t& size)
{
	uint32_t pos = 0;
	std::string currentLine;
	std::string prefix;
	std::vector<Core::Components::Mesh> Meshes;

	while (pos != size)
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		prefix = currentLine.substr(0, 2);
		if (prefix == "o ")
		{
			Meshes.push_back(Core::Components::Mesh());
		}
		else if (prefix == "mt")
		{
			std::string path = currentLine;
			MaterialLoop(path);
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
			auto indices = Utils::Loader::GetIndices(currentLine);
			for (size_t i = 0; i < 3; i++)
			{
				Meshes.back().Indices.push_back(indices[i]);
			}
		}
		else if (prefix == "us")
		{
			std::string MaterialName = Utils::Loader::GetString(currentLine);
			if (auto mat = Resources::ResourceManager::Get<Resources::Material>(MaterialName.c_str())) {
				Meshes.back().Materials.push_back(mat);
			}
			else
			{
				mat = Resources::ResourceManager::Get<Resources::Material>("DefaultMaterial");
				Meshes.back().Materials.push_back(mat);
			}
		}
		pos++;
	}
	for (auto mesh : Meshes)
	{
		auto MeshNode = new Core::Node();
		MeshNode->AddComponent(std::make_shared<Core::Components::Mesh>(mesh));
		this->AddChildren(MeshNode);
	}
}

void Resources::Model::MaterialLoop(std::string path)
{
	// Read File.
	path = path.substr(path.find_first_of(' ') + 1);
	path = path.substr(0, path.size() - 1);
	path = _path.substr(0, _path.find_last_of('/') + 1) + path;
	bool sucess;
	uint32_t size = 0;
	auto data = Utils::Loader::ReadFile(path.c_str(), size, sucess);
	if (!sucess)
		return;


	std::string currentLine;
	std::string prefix;
	uint32_t pos = 0;
	Resources::Material* currentMaterial = nullptr;;
	while (pos != size)
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		prefix = currentLine.substr(0, 2);
		if (prefix == "ne")
		{
			currentMaterial = new Resources::Material();
			auto name = currentLine.substr(currentLine.find_first_of(' ') + 1);
			name = name.substr(0, name.size() - 1);
			currentMaterial->SetName(name);
			ResourceManager::Add(name, currentMaterial);
		}
		else if (prefix == "Ka")
		{
			currentMaterial->_ambient = Utils::Loader::GetVec3(currentLine);
		}
		else if (prefix == "Kd")
		{
			currentMaterial->_diffuse = Utils::Loader::GetVec3(currentLine);
		}
		else if (prefix == "Ks")
		{
			currentMaterial->_specular = Utils::Loader::GetVec3(currentLine);
		}
		else if (prefix == "Ns")
		{
			currentMaterial->_shininess = Utils::Loader::GetFloat(currentLine);
		}
		else if (prefix == "d ")
		{
			float transparency = Utils::Loader::GetFloat(currentLine);
			currentMaterial->_ambient.w = transparency;
			currentMaterial->_diffuse.w = transparency;
			currentMaterial->_specular.w = transparency;
		}
		else if (prefix == "ma")
		{
			std::string texPath = Utils::Loader::GetString(currentLine);
			if (auto texture = Resources::ResourceManager::GetWithPath<Resources::Texture>(texPath.c_str()))
			{
				currentMaterial->_texture = texture;
			}
			else
			{
				texture = Resources::ResourceManager::Create<Resources::Texture>(texPath);
				currentMaterial->_texture = texture;
			}
		}
		pos++;
	}
	delete[] data;
}
