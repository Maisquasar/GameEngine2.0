#include "..\..\Include\Resources\Material.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/Utils/Loader.h"
#include "Include/App.h"

Resources::Material::Material()
{
	_shader = Resources::ResourceManager::GetDefaultShader();
	_initialized = true;
}

Resources::Material::~Material()
{
}

void Resources::Material::ShowInInspector()
{
	ImGui::Text("%s (Material)", GetName().c_str());
	ImGui::Text("Path : %s", GetPath().c_str());
	ImGui::Separator();

	// Shader
	if (ImGui::Button("Shader"))
	{
		ImGui::OpenPopup("ShaderPopup");
	}
	if (auto sha = Resources::ResourceManager::ResourcesPopup<Resources::Shader>("ShaderPopup"))
		SetShader(sha);
	ImGui::SameLine();
	ImGui::Text("%s", GetShader()->GetName().c_str());
	ImGui::NewLine();

	// Texture
	if (ImGui::Button("Change Texture"))
	{
		ImGui::OpenPopup("TexturePopup");
	}
	if (auto tex = Resources::ResourceManager::ResourcesPopup<Resources::Texture>("TexturePopup"))
		SetTexture(tex);
	ImGui::SameLine();
	if (GetTexture())
	{
		ImGui::Text(GetTexture()->GetName().c_str());
	}
	else
		ImGui::Text("None");
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		SetTexture(nullptr);
	}
	ImGui::NewLine();

	// Parameters
	ImGui::ColorEdit4("Ambient", &(*GetPtrAmbient())[0]);
	ImGui::NewLine();

	ImGui::ColorEdit4("Diffuse", &(*GetPtrDiffuse())[0]);
	ImGui::NewLine();

	ImGui::ColorEdit4("Specular", &(*GetPtrSpecular())[0]);
	ImGui::NewLine();

	if (ImGui::Button("Save"))
	{
		Utils::Loader::WriteMaterial(this);
	}
}

void Resources::Material::Load(std::string filename)
{
	App::ThreadManager.QueueJob(&Material::MultiThreadLoad, this, filename);
}

void Resources::Material::MultiThreadLoad(std::string filename)
{
	uint32_t size = 0;
	bool sucess = false;
	auto data = Utils::Loader::ReadFile(_path.c_str(), size, sucess);
	if (!sucess)
		return;
	uint32_t pos = 0;
	while (pos != size)
	{
		auto currentLine = Utils::Loader::GetLine(data, pos);
		auto prefix = currentLine.substr(0, 3);
		if (prefix == "Edi")
		{
			bool editable = Utils::Loader::GetInt(currentLine);
			SetEditable(editable);
		}
		else if (prefix == "Sha")
		{
			auto shaderPath = Utils::Loader::GetString(currentLine);
			SetShader(Resources::ResourceManager::Get<Resources::Shader>(shaderPath.c_str()));
		}
		else if (prefix == "Tex")
		{
			auto texturePath = Utils::Loader::GetString(currentLine);
			SetTexture(Resources::ResourceManager::Get<Resources::Texture>(texturePath.c_str()));
		}
		else if (prefix == "Amb")
		{
			auto amb = Utils::Loader::GetVec4(currentLine);
			SetAmbient(amb);
		}
		else if (prefix == "Dif")
		{
			auto dif = Utils::Loader::GetVec4(currentLine);
			SetDiffuse(dif);
		}
		else if (prefix == "Spe")
		{
			auto spe = Utils::Loader::GetVec4(currentLine);
			SetSpecular(spe);
		}
		else if (prefix == "Shi")
		{
			auto shi = Utils::Loader::GetFloat(currentLine);
			SetShiniess(shi);
			break;
		}
		pos++;
	}
	SetInitialized();
	LOG(Debug::LogType::INFO, "Successfully loaded Material : %s", _path.c_str());
	delete[] data;
}
