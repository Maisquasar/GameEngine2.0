#include "..\..\Include\Utils\Loader.h"

void Utils::Loader::SkipLine(const char* data, uint32_t& pos)
{
	while (data[pos] != '\0' && data[pos] != '\n')
	{
		pos++;
	}
	pos++;
}

std::string Utils::Loader::GetString(const char* data, uint32_t& pos, int dec)
{
	pos += dec;
	std::string value;
	while (data[pos] != '\n' && data[pos] != '\0' && data[pos] != '\r' && data[pos] != '\t')
	{
		value.push_back(data[pos]);
		pos++;
	}
	return value;
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

float Utils::Loader::GetFloat(const char* data, uint32_t& pos, int dec)
{
	pos += dec;
	std::string value;
	while (data[pos] != '\n' && data[pos] != '\0' && data[pos] != '\r')
	{
		value.push_back(data[pos]);
		pos++;
	}
	return std::stof(value);
}

Math::Vector4 Utils::Loader::GetVec4(const char* data, uint32_t& pos, int dec)
{
	Math::Vector4 position;
	std::string value;
	pos += dec;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
	{
		if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
			value.push_back(data[pos]);
		pos++;
	}
	position.x = std::stof(value);
	value.clear();
	pos += 2;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
	{
		if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
			value.push_back(data[pos]);
		pos++;
	}
	position.y = std::stof(value);
	value.clear();
	pos += 2;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
	{
		if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
			value.push_back(data[pos]);
		pos++;
	}
	position.z = std::stof(value);
	value.clear();
	pos += 2;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
	{
		if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
			value.push_back(data[pos]);
		pos++;
	}
	position.w = std::stof(value);
	value.clear();

	return position;
}

Math::Vector3 Utils::Loader::GetVec3(const char* data, uint32_t& pos, int dec)
{
	Math::Vector3 position;
	std::string value;
	pos += dec;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
	{
		if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
			value.push_back(data[pos]);
		pos++;
	}
	position.x = std::stof(value);
	value.clear();
	pos += 2;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
	{
		if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
			value.push_back(data[pos]);
		pos++;
	}
	position.y = std::stof(value);
	value.clear();
	pos += 2;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
	{
		if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
			value.push_back(data[pos]);
		pos++;
	}
	position.z = std::stof(value);
	value.clear();

	return position;
}

Math::Vector2 Utils::Loader::GetVec2(const char* data, uint32_t& pos, int dec)
{
	Math::Vector2 vector;
	std::string value;
	pos += dec;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
	{
		if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
			value.push_back(data[pos]);
		pos++;
	}
	vector.x = std::stof(value);
	value.clear();
	pos += 2;

	while (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
	{
		if (data[pos] != '\0' && data[pos] != '\n' && data[pos] != ' ' && data[pos] != ',')
			value.push_back(data[pos]);
		pos++;
	}
	vector.y = std::stof(value);
	value.clear();
	return vector;
}

Math::Matrix4 Utils::Loader::GetMat4(const char* data, uint32_t& pos, int dec)
{
	pos += dec;
	Math::Matrix4 mat;
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
