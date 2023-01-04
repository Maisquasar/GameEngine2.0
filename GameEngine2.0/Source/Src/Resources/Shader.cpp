#include "Include/Resources/Shader.h"
std::string ReadFile(std::filesystem::path const& filename);

Resources::Shader::Shader() {}
Resources::Shader::~Shader()
{
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	glDeleteProgram(Program);
}

bool Resources::Shader::SetShader(std::string filename)
{
	filename += "/vertex.glsl";
	std::string tmp = ReadFile(filename);
	const char* vertexShaderSources = tmp.c_str();
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vertexShaderSources, NULL);
	glCompileShader(VertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
		LOG(Debug::LogType::L_ERROR, "ERROR::SHADER::VERTEX::COMPILATION_FAILED %s", infoLog);
		return false;
	}
	else
		LOG(Debug::LogType::INFO, "Successfully Compiling Shader: %s", filename.c_str());
	return true;
}
bool Resources::Shader::SetFragmentShader(std::string filename)
{
	filename += "/fragment.glsl";
	std::string tmp = ReadFile(filename);
	const char* fragmentShaderSource = tmp.c_str();
	int success;
	char infoLog[512];
	// fragment shader
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(FragmentShader);
	// check for shader compile errors
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
		LOG(Debug::LogType::L_ERROR, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED %s", infoLog);
		return false;
	}
	else
		LOG(Debug::LogType::INFO, "Successfully Compiling Shader: %s", filename.c_str());

	return true;
}

void Resources::Shader::Load(std::string filename)
{
	if (SetShader(_path) && SetFragmentShader(_path))
	{
		if (Link())
			Loaded = true;
	}
}

void Resources::Shader::Recompile()
{
	SetShader(this->GetPath());
	SetFragmentShader(this->GetPath());
	Link();
	PrintLog("Shader %s Recompiled !", this->GetPath().c_str());
}


bool Resources::Shader::Link()
{
	// link shaders
	Program = glCreateProgram();
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glLinkProgram(Program);
	// check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(Program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
		LOG(Debug::LogType::L_ERROR, "ERROR::SHADER::PROGRAM::LINKING_FAILED %s", infoLog);
		return false;
	}
	else {
		LOG(Debug::LogType::INFO, "Successfully link Shader %s", _name.c_str());
		_linked = true;
	}

	Initialize();
	return true;
}

void Resources::Shader::Initialize()
{
	glUseProgram(this->Program);
	_location[Location::L_MVP] = glGetUniformLocation(Program, "MVP");
	_location[Location::L_M] = glGetUniformLocation(Program, "M");
	_location[Location::L_TIME] = glGetUniformLocation(Program, "time");
	_location[Location::L_ENABLE_TEXTURE] = glGetUniformLocation(Program, "EnableTexture");
	_location[Location::L_TEXTURE] = glGetUniformLocation(Program, "Tex");
	_location[Location::L_MATERIAL] = glGetUniformLocation(Program, "Mat");
	_location[Location::L_NORMAL_MAP] = glGetUniformLocation(Program, "NormalMap");
	_location[Location::L_DIRECTIONAL] = glGetUniformLocation(Program, "DirLight");
	_location[Location::L_POINT] = glGetUniformLocation(Program, "PointLight");
	_location[Location::L_SPOT] = glGetUniformLocation(Program, "SpotLight");
	_location[Location::L_COLOR] = glGetUniformLocation(Program, "OurColor");
	_location[Location::L_PROJECTIONMATRIX] = glGetUniformLocation(Program, "ProjectionMatrix");
	_location[Location::L_MODELVIEWMATRIX] = glGetUniformLocation(Program, "ModelViewMatrix");
	_location[Location::L_SKINNINGMATRICES] = glGetUniformLocation(Program, "SkinningMatrices");
	_location[Location::L_MAXBONEWEIGHT] = glGetUniformLocation(Program, "MaxBoneWeight");
	_initialized = true;
}

int Resources::Shader::GetLocation(Location loc)
{
	return _location[loc];
}

std::string ReadFile(std::filesystem::path const& filename)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string Code;
	std::ifstream ShaderFile;
	// ensure ifstream objects can throw exceptions:
	ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		ShaderFile.open(filename);
		std::stringstream ShaderStream;
		// read file's buffer contents into streams
		ShaderStream << ShaderFile.rdbuf();
		// close file handlers
		ShaderFile.close();
		// convert stream into string
		Code = ShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return Code;
}