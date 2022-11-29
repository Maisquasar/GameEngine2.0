#include "..\..\Include\Utils\Loader.h"
#include "Include/Resources/ResourceManager.h"

void Utils::Loader::SkipLine(const char* data, uint32_t& pos)
{
	while (data[pos] != '\0' && data[pos] != '\n')
	{
		pos++;
	}
	pos++;
}

std::string Utils::Loader::GetLine(const char* data, uint32_t& pos)
{
	std::string line;
	while (data[pos] != '\0' && data[pos] != '\n')
	{
		line.push_back(data[pos]);
		pos++;
	}
	if (line[0] == '\t')
		line = line.substr(line.find_first_not_of('\t'));
	return line;
}

std::string Utils::Loader::GetString(std::string line)
{
	line = line.substr(line.find_first_of(' '));
	line = line.substr(line.find_first_of(':') + 2);
	line = line.substr(0, line.find_first_of('\r'));
	line = line.substr(0, line.find_first_of('\n'));
	return line;
}

int Utils::Loader::GetInt(std::string line)
{
	float out;
	std::string value;
	std::string temp = line;
	temp = temp.substr(temp.find_first_of(' '));
	temp = temp.substr(temp.find_first_of(':') + 2);
	value = temp.substr(0, temp.find_first_of('\n'));
	value = value.substr(0, value.find_first_of('\r'));
	out = std::stoi(value);
	return out;
}

float Utils::Loader::GetFloat(std::string line)
{
	float out;
	std::string value;
	std::string temp = line;
	temp = temp.substr(temp.find_first_of(' '));
	temp = temp.substr(temp.find_first_of(':') + 2);
	value = temp.substr(0, temp.find_first_of('\n') - 1);
	value = value.substr(0, value.find_first_of('\r') - 1);
	out = std::stof(value);
	return out;
}

std::vector<Math::Integer3> Utils::Loader::GetIndices(std::string line)
{
	std::vector<Math::Integer3> out;
	out.resize(3);
	std::string value;
	std::string temp = line.substr(2);
	std::string temp2;
	for (size_t j = 0; j < 3; j++)
	{
		temp2 = temp.substr(temp.find_first_not_of(' '));
		temp2 = temp2.substr(0, temp2.find_first_of(' '));
		for (size_t i = 0; i < 3; i++)
		{
			value = temp2.substr(0, temp2.find_first_of('/'));
			out[j][i] = std::stoi(value) - 1;
			temp2 = temp2.substr(temp2.find_first_of('/') + 1);
		}
		temp = temp.substr(temp.find_first_of(' ') + 1);
	}
	return out;
}

Math::Vector4 Utils::Loader::GetVec4(std::string line)
{
	Math::Vector4 out;
	std::string value;
	std::string temp = line.substr(line.find_first_of(':') + 2);
	for (size_t i = 0; i < 4; i++)
	{
		value = temp.substr(0, temp.find_first_of(' '));
		out[i] = std::stof(value);
		temp = temp.substr(temp.find_first_of(' ') + 1);
	}
	return out;
}

Math::Vector3 Utils::Loader::GetVector3(std::string line)
{
	Math::Vector3 out;
	std::string value;
	std::string temp = line.substr(line.find_first_of(':') + 2);
	for (size_t i = 0; i < 3; i++)
	{
		value = temp.substr(0, temp.find_first_of(' '));
		out[i] = std::stof(value);
		temp = temp.substr(temp.find_first_of(' ') + 1);
	}
	return out;
}

Math::Vector3 Utils::Loader::GetVec3(std::string line)
{
	Math::Vector3 out;
	std::string value;
	std::string temp = line.substr(2);
	temp = temp.substr(temp.find_first_not_of(' '));
	for (size_t i = 0; i < 3; i++)
	{
		value = temp.substr(0, temp.find_first_of(' '));
		out[i] = std::stof(value);
		temp = temp.substr(temp.find_first_of(' ') + 1);
	}
	return out;
}

Math::Vector2 Utils::Loader::GetVec2(std::string line)
{
	Math::Vector2 out;
	std::string value;
	std::string temp = line.substr(2);
	temp = temp.substr(temp.find_first_not_of(' '));
	for (size_t i = 0; i < 2; i++)
	{
		value = temp.substr(0, temp.find_first_of(' '));
		out[i] = std::stof(value);
		temp = temp.substr(temp.find_first_not_of(' '));
	}
	return out;
}

Math::Matrix4 Utils::Loader::GetMat4(const char* data, uint32_t& pos, int dec)
{
	Math::Matrix4 mat;
	pos += dec;
	std::string value;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',' && data[pos] != '}')
			{
				if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',' && data[pos] != '}')
					value.push_back(data[pos]);
				pos++;
			}
			mat.content[i][j] = std::stof(value);
			value.clear();
			pos++;
		}
		pos += 2;
	}
	return mat;
}

const char* Utils::Loader::ReadFile(const char* filename, uint32_t& size, bool& success)
{
	std::ifstream is(filename, std::ifstream::binary);
	if (is) {
		success = true;
		// get length of file:
		is.seekg(0, is.end);
		uint32_t length = (uint32_t)is.tellg();
		is.seekg(0, is.beg);

		char* buffer = new char[length];

		// read data as a block:
		is.read(buffer, length);
		is.close();
		size = length;
		return buffer;
	}
	else
	{
		success = false;
		LOG(Debug::LogType::L_ERROR, "File %s cannot be found", filename);
		return 0;
	}
}

void Utils::Loader::MtlLoader(std::string path)
{
	// Read File.
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
			auto name = currentLine.substr(currentLine.find_first_of(' ') + 1);
			path = path.substr(0, path.find_last_of('/')) + '/' + name + ".mat";

			if (auto mat = Resources::ResourceManager::Get<Resources::Material>((path).c_str()))
			{
				continue;
			}
			if (currentMaterial)
				WriteMaterial(currentMaterial);

			currentMaterial = new Resources::Material();
			currentMaterial->SetName(name);
			currentMaterial->SetPath(path);
			Resources::ResourceManager::Add(path, currentMaterial);
			LOG(Debug::LogType::INFO, "Sucessfuly Loaded Material %s from MTL !", currentMaterial->GetPath().c_str())
		}
		else if (!currentMaterial)
		{

		}
		else if (prefix == "Ka")
		{
			currentMaterial->SetAmbient(Utils::Loader::GetVec3(currentLine));
		}
		else if (prefix == "Kd")
		{
			currentMaterial->SetDiffuse(Utils::Loader::GetVec3(currentLine));
		}
		else if (prefix == "Ks")
		{
			currentMaterial->SetSpecular(Utils::Loader::GetVec3(currentLine));
		}
		else if (prefix == "Ns")
		{
			currentMaterial->SetShiniess(Utils::Loader::GetFloat(currentLine));
		}
		else if (prefix == "d ")
		{
			float transparency = Utils::Loader::GetFloat(currentLine);
			currentMaterial->SetTransparency(transparency);
		}
		else if (prefix == "ma")
		{
			std::string texPath = Utils::Loader::GetString(currentLine);
			if (auto texture = Resources::ResourceManager::Get<Resources::Texture>(texPath.c_str()))
			{
				currentMaterial->SetTexture(texture);
			}
			else
			{
				texture = Resources::ResourceManager::Create<Resources::Texture>(texPath);
				currentMaterial->SetTexture(texture);
			}
		}
		pos++;
	}
	if (currentMaterial)
		WriteMaterial(currentMaterial);
	delete[] data;
}

void Utils::Loader::WriteMaterial(Resources::Material* material)
{
	std::string output;
	output = StringFormat("Name : %s\n", material->GetName().c_str());
	output += StringFormat("Editable : %d\n", material->IsEditable());
	if (material->GetShader())
		output += StringFormat("Shader : %s\n", material->GetShader()->GetPath().c_str());
	if (material->GetTexture())
		output += StringFormat("Texture : %s\n", material->GetTexture()->GetPath().c_str());

	output += StringFormat("Ambient : %s\n", material->GetAmbient().ToString().c_str());
	output += StringFormat("Diffuse : %s\n", material->GetDiffuse().ToString().c_str());
	output += StringFormat("Specular : %s\n", material->GetSpecular().ToString().c_str());
	output += StringFormat("Shininess : %f\n", material->GetShiniess());

	FILE* file;
	fopen_s(&file, material->GetPath().c_str(), "w");
	if (file)
	{
		LOG(Debug::LogType::INFO, "Creating File Material %s!", material->GetPath().c_str());
		fputs(output.c_str(), file);
		fclose(file);
	}
}

void Utils::Loader::LoadMaterial(std::string path)
{
	uint32_t size = 0;
	bool sucess = false;
	auto data = Utils::Loader::ReadFile(path.c_str(), size, sucess);
	if (!sucess)
		return;
	LOG(Debug::LogType::INFO, "Loading Material : %s", path.c_str());
	uint32_t pos = 0;
	Resources::Material* material = new Resources::Material();
	material->SetPath(path);
	auto name = path.substr(path.find_last_of('/') + 1);
	material->SetName(name.substr(0, name.size() - 3));
	while (pos != size)
	{
		auto currentLine = Utils::Loader::GetLine(data, pos);
		auto prefix = currentLine.substr(0, 3);
		if (prefix == "Nam")
		{
			material->SetName(Utils::Loader::GetString(currentLine));
		}
		else if (prefix == "Edi")
		{
			bool editable = Utils::Loader::GetInt(currentLine);
			material->SetEditable(editable);
		}
		else if (prefix == "Sha")
		{
			auto shaderPath = Utils::Loader::GetString(currentLine);
			material->SetShader(Resources::ResourceManager::Get<Resources::Shader>(shaderPath.c_str()));
		}
		else if (prefix == "Tex")
		{
			auto texturePath = Utils::Loader::GetString(currentLine);
			material->SetTexture(Resources::ResourceManager::Get<Resources::Texture>(texturePath.c_str()));
		}
		else if (prefix == "Amb")
		{
			auto amb = Utils::Loader::GetVec4(currentLine);
			material->SetAmbient(amb);
		}
		else if (prefix == "Dif")
		{
			auto dif = Utils::Loader::GetVec4(currentLine);
			material->SetDiffuse(dif);
		}
		else if (prefix == "Spe")
		{
			auto spe = Utils::Loader::GetVec4(currentLine);
			material->SetSpecular(spe);
		}
		else if (prefix == "Shi")
		{
			auto shi = Utils::Loader::GetFloat(currentLine);
			material->SetShiniess(shi);
			break;
		}
		pos++;
	}
	Resources::ResourceManager::Add(material->GetPath(), material);
	LOG(Debug::LogType::INFO, "Successfully loaded Material : %s", path.c_str());
	delete[] data;
}
