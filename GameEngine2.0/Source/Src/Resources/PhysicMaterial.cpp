#include "stdafx.h"
#include "Include/Resources/PhysicMaterial.h"
#include "Include/Utils/Loader.h"
#include "Include/App.h"

Resources::PhysicMaterial::PhysicMaterial()
{
}

Resources::PhysicMaterial::~PhysicMaterial()
{
}

void Resources::PhysicMaterial::Load()
{
#if MULTITHREAD_LOADING
	Application.ThreadManager.QueueJob(&Resources::PhysicMaterial::MultiThreadLoad, this);
#else
	MultiThreadLoad();
#endif
}

void Resources::PhysicMaterial::MultiThreadLoad()
{
	uint32_t size;
	bool sucess;
	auto data = Utils::Loader::ReadFile(_path.c_str(), size, sucess);
	uint32_t pos = 0;
	if (!sucess)
	{
		PrintError("Failed to load %s", _path.c_str());
	}
	std::string currentLine;
	std::string prefix;
	while (pos != size)
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		prefix = currentLine.substr(0, 2);
		if (prefix == "sf")
		{
			_staticFriction = Utils::Loader::GetFloat(currentLine);
		}
		else if (prefix == "df")
		{
			_dynamicFriction = Utils::Loader::GetFloat(currentLine);
		}
		else if (prefix == "re")
		{
			_restitution = Utils::Loader::GetFloat(currentLine);
			break;
		}
		pos++;
	}
	delete[] data;
	Loaded = true;
	Initialize();
}

void Resources::PhysicMaterial::Save()
{
	std::string output;
	output += Utils::StringFormat("sf : %f\n", _staticFriction);
	output += Utils::StringFormat("df : %f\n", _dynamicFriction);
	output += Utils::StringFormat("re : %f\n", _restitution);

	FILE* file;
	fopen_s(&file, GetPath().c_str(), "w");
	if (file)
	{
		LOG(Debug::LogType::INFO, "Creating File Physic Material %s!", GetPath().c_str());
		fputs(output.c_str(), file);
		fclose(file);
	}
}

void Resources::PhysicMaterial::Initialize()
{
	_physxMaterial = Application.GetPhysicHandler()->GetPhysics()->createMaterial(_staticFriction, _dynamicFriction, _restitution);
	_initialized = true;
}

void Resources::PhysicMaterial::ShowInInspector()
{
	ImGui::Text("%s (Physic Material)", GetName().c_str());
	ImGui::Text("Path : %s", GetPath().c_str());
	ImGui::Separator();
	if (ImGui::InputFloat("Static Friction", &_staticFriction, 0))
		_physxMaterial->setStaticFriction(_staticFriction);
	if (ImGui::InputFloat("Dynamic Friction", &_dynamicFriction, 0))
		_physxMaterial->setDynamicFriction(_staticFriction);
	if (ImGui::InputFloat("Restitution", &_restitution, 0))
		_physxMaterial->setRestitution(_restitution);

	if (ImGui::Button("Save"))
		Save();
}
