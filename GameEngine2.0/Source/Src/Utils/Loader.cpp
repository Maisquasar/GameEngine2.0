#include "..\..\Include\Utils\Loader.h"

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
	line.push_back('\n');
	return line;
}

std::string Utils::Loader::GetString(std::string line)
{
	return line.substr(line.find_first_of(' ') + 1);
}

int Utils::Loader::GetInt(const char* data, uint32_t& pos, int dec)
{
	pos += dec;
	std::string value;
	while (data[pos] != '\n' && data[pos] != '\0' && data[pos] != '\r')
	{
		value.push_back(data[pos]);
		pos++;
	}
	return std::stoi(value);
}

float Utils::Loader::GetFloat(std::string line)
{
	float out;
	std::string value;
	std::string temp = line.substr(2);
	temp = temp.substr(temp.find_first_not_of(' '));
	value = temp.substr(0, temp.find_first_of(' '));
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
	std::string temp = line.substr(2);
	temp = temp.substr(temp.find_first_not_of(' '));
	for (size_t i = 0; i < 4; i++)
	{
		value = temp.substr(0, temp.find_first_of(' '));
		out[i] = std::stof(value);
		temp = temp.substr(temp.find_first_not_of(' '));
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
