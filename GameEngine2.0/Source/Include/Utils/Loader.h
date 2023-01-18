#pragma once
#include "Include/Debug/Log.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Include/Math/Math.h"
#include "Utils.h"

namespace Resources
{
	class Material;
	class SkeletalMesh;
	class Mesh;
	class IResource;
	class Model;
}

namespace ofbx
{
	struct IScene;
	struct Mesh;
	struct NodeAttribute;
	struct Skin;
	struct AnimationStack;
}

namespace Utils::Loader
{
	void SkipLine(const char* data, uint32_t& pos);
	std::string GetLine(const char* data, uint32_t& pos);
	std::string GetString(std::string);
	int GetInt(std::string line);
	float GetFloat(std::string line);
	Math::Vec4 GetVec4(std::string);
	// Obj Loading Vec3
	Math::Vec3 GetVec3(std::string);
	// Scene Loading Vec3
	Math::Vec3 GetVector3(std::string);
	// Obj Loading Vec2
	Math::Vec2 GetVec2(std::string);
	// Scene Loading Vec2
	Math::Vec2 GetVector2(std::string);
	std::vector<Math::IVec3> GetIndices(std::string);
	Math::Mat4 GetMat4(std::string line);

	const char* ReadFile(const char* filename, uint32_t& size, bool& success);

	void MtlLoader(std::string path);

	void WriteMaterial(Resources::Material* material);

	void LoadMaterial(std::string path);

	void FBXLoad(std::string path);
	void LoadMesh(const ofbx::Mesh* mesh, std::string path, Resources::Model*);
	void LoadSkeleton(const ofbx::Skin* Skel, std::string path, Resources::SkeletalMesh* res,int index_count, Resources::Model*);
	void LoadAnimation(const ofbx::AnimationStack* stack, std::string path);
}