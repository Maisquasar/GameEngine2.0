#include "..\..\Include\Resources\Material.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/Utils/Loader.h"

Resources::Material::Material()
{
	_shader = Resources::ResourceManager::Get<Resources::Shader>("Assets/Default/Shaders/UnlitShader");
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
