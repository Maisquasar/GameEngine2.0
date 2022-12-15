#include "..\..\Include\Utils\Loader.h"
#include "Include/App.h"

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
	int out;
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
	value.clear();
	temp.clear();
	temp2.clear();
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
		temp = temp.substr(temp.find_first_of(' ') + 1);
	}
	out = Math::Vector2(out.x, 1 - out.y);
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
		LOG(Debug::LogType::WARNING, "File %s cannot be found", filename);
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

			if (auto mat = Application.GetResourceManager()->Get<Resources::Material>((path).c_str()))
			{
				continue;
			}
			if (currentMaterial)
				WriteMaterial(currentMaterial);

			currentMaterial = new Resources::Material();
			currentMaterial->SetName(name);
			currentMaterial->SetPath(path);
			Application.GetResourceManager()->Add(path, currentMaterial);
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
			if (auto texture = Application.GetResourceManager()->Get<Resources::Texture>(texPath.c_str()))
			{
				currentMaterial->SetTexture(texture);
			}
			else
			{
				texture = Application.GetResourceManager()->Create<Resources::Texture>(texPath);
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
		if (prefix == "Edi")
		{
			bool editable = Utils::Loader::GetInt(currentLine);
			material->SetEditable(editable);
		}
		else if (prefix == "Sha")
		{
			auto shaderPath = Utils::Loader::GetString(currentLine);
			material->SetShader(Application.GetResourceManager()->Get<Resources::Shader>(shaderPath.c_str()));
		}
		else if (prefix == "Tex")
		{
			auto texturePath = Utils::Loader::GetString(currentLine);
			material->SetTexture(Application.GetResourceManager()->Get<Resources::Texture>(texturePath.c_str()));
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
	material->SetInitialized();
	Application.GetResourceManager()->Add(material->GetPath(), material);
	LOG(Debug::LogType::INFO, "Successfully loaded Material : %s", path.c_str());
	delete[] data;
}

#include <OpenFBX/ofbx.h>
void Utils::Loader::FBXLoad(std::string path)
{
	uint32_t size;
	bool sucess;
	auto data = (ofbx::u8*)Utils::Loader::ReadFile(path.c_str(), size, sucess);
	if (!sucess) {
		delete[] data;
		return;
	}
	ofbx::IScene* Scene = ofbx::load(data, size, (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
	if (Scene) {
		int count = Scene->getMeshCount();
		for (int i = 0; i < count; i++)
			LoadMesh(Scene->getMesh(i), path);

		for (int i = 0, n = Scene->getAnimationStackCount(); i < n; ++i)
		{
			LoadAnimation(Scene->getAnimationStack(i), path);
		}
	}
	delete[] data;
	Scene->destroy();
}

void Utils::Loader::LoadMesh(const ofbx::Mesh* mesh, std::string path)
{
	// Set-Up Mesh
	auto Mesh = new Resources::Mesh();
	Mesh->SetName(mesh->name);
	Mesh->SetPath(path + "::" + Mesh->GetName());

	// Vertices Loading.
	auto sca = mesh->getLocalScaling();
	for (int i = 0; i < mesh->getGeometry()->getVertexCount(); i++)
	{
		auto vec = mesh->getGeometry()->getVertices()[i];
		Mesh->Positions.push_back({ (float)vec.x * (float)sca.x, (float)vec.y * (float)sca.y, (float)vec.z * (float)sca.z });
	}

	const ofbx::Vec3* normals = mesh->getGeometry()->getNormals();
	const ofbx::Vec2* uvs = mesh->getGeometry()->getUVs();
	int count = mesh->getGeometry()->getIndexCount();
	for (int i = 0; i < count; ++i)
	{
		ofbx::Vec3 n = normals[i];
		ofbx::Vec2 uv = uvs[i];
		Mesh->Normals.push_back({ (float)n.x, (float)n.y, (float)n.z });
		Mesh->TextureUVs.push_back({ (float)uv.x, (float)uv.y });

	}

	// Material Loop.
	size_t lastMaterial = 0;
	size_t lastIndex = 0;
	if (mesh->getMaterialCount() > 1) {
		for (size_t i = 0; i < count / 3; i++)
		{
			if (lastMaterial != mesh->getGeometry()->getMaterials()[i]) {
				auto mat = new Resources::Material();
				auto mesh_mat = mesh->getMaterial((int)lastMaterial)->getDiffuseColor();
				mat->SetShader(Application.GetResourceManager()->GetDefaultShader());
				mat->SetDiffuse(Math::Vector4(mesh_mat.r, mesh_mat.g, mesh_mat.b, 1.f));
				Mesh->SubMeshes.push_back(Resources::SubMesh());
				Mesh->SubMeshes.back().Material = mat;
				Mesh->SubMeshes.back().StartIndex = lastIndex;
				Mesh->SubMeshes.back().Count = (i * 3 - lastIndex);
				lastIndex = i * 3;
				lastMaterial = mesh->getGeometry()->getMaterials()[i];
			}
		}
	}
	auto mat = new Resources::Material();
	auto mesh_mat = mesh->getMaterial((int)lastMaterial)->getDiffuseColor();
	mat->SetShader(Application.GetResourceManager()->GetDefaultShader());
	mat->SetDiffuse(Math::Vector4(mesh_mat.r, mesh_mat.g, mesh_mat.b));
	Mesh->SubMeshes.push_back(Resources::SubMesh());
	Mesh->SubMeshes.back().Material = mat;
	Mesh->SubMeshes.back().StartIndex = lastIndex;
	Mesh->SubMeshes.back().Count = (size_t)mesh->getGeometry()->getIndexCount() - lastIndex;



	// Set-up all Vertices.
	const int* indices = mesh->getGeometry()->getFaceIndices();
	int index_count = mesh->getGeometry()->getIndexCount();


	for (int i = 0; i < index_count; i++)
	{
		Mesh->Vertices.push_back(Mesh->Positions[i].x);
		Mesh->Vertices.push_back(Mesh->Positions[i].y);
		Mesh->Vertices.push_back(Mesh->Positions[i].z);
		Mesh->Vertices.push_back(Mesh->Normals[i].x);
		Mesh->Vertices.push_back(Mesh->Normals[i].y);
		Mesh->Vertices.push_back(Mesh->Normals[i].z);
		Mesh->Vertices.push_back(Mesh->TextureUVs[i].x);
		Mesh->Vertices.push_back(Mesh->TextureUVs[i].y);
		Mesh->Vertices.push_back(0);
		Mesh->Vertices.push_back(0);
		Mesh->Vertices.push_back(0);
	}
	int mat_count = mesh->getMaterialCount();

	Application.GetResourceManager()->Add(Mesh->GetPath(), Mesh);
	Mesh->Loaded = true;
	Mesh->Initialize();

	if (auto skin = mesh->getGeometry()->getSkin())
		LoadSkeleton(skin, path + "::" + Mesh->GetName());
}
#include "Include/Resources/Skeleton.h"
#define FBXScale 0.01f
void Utils::Loader::LoadSkeleton(const ofbx::Skin* Skel, std::string path)
{
	auto NewSkel = new Resources::Skeleton();
	auto name = path.substr(path.find_last_of(':') + 1);
	name = name + "::" + "Skel";
	path = path + "::" + "Skel";
	NewSkel->SetPath(path);
	NewSkel->SetName(name);
	Bone* root = nullptr;
	std::map<std::string, Bone*> Bones;
	for (int i = 0; i < Skel->getClusterCount(); i++)
	{
		auto link = Skel->getCluster(i)->getLink();
		Bone* bone = new Bone();
		bone->Name = link->name;
		bone->Id = i;

		// Set Up Transform
		auto pos = link->getLocalTranslation();
		auto rot = link->getPreRotation();
		auto sca = link->getLocalScaling();

		Math::Vector3 vecPos = Math::Vector3((float)pos.x, (float)pos.y, (float)pos.z);
		Math::Vector3 vecRot = Math::Vector3((float)rot.x, (float)rot.y, (float)rot.z);
		Math::Vector3 vecSca = Math::Vector3((float)sca.x, (float)sca.y, (float)sca.z);

		bone->Transform.SetLocalPosition(vecPos);
		bone->Transform.SetLocalRotation(vecRot.ToQuaternion());
		bone->Transform.SetLocalScale(vecSca);

		if (i != 0) { bone->SetParent(Bones[link->getParent()->name]); }
		else { root = bone; }

		Bones[bone->Name] = bone;
	}
	if (root)
		NewSkel->RootBone = root;

	Application.GetResourceManager()->Add(NewSkel->GetPath(), NewSkel);
}

#include "Include/Resources/Animation.h"

void Utils::Loader::LoadAnimation(const ofbx::AnimationStack* stack, std::string path)
{
	for (int j = 0; stack->getLayer(j); ++j)
	{
		const ofbx::AnimationLayer* layer = stack->getLayer(j);
		Resources::Animation* Animation = new Resources::Animation();
		auto name = path.substr(path.find_last_of('/') + 1);
		name = name + "::" + "Anim";
		path = path + "::" + "Anim";
		Animation->SetPath(path);
		Animation->SetName(name);

		printf("Layer %s %d\n", layer->name, j);
		for (int k = 0; layer->getCurveNode(k); ++k)
		{
			const ofbx::AnimationCurveNode* node = layer->getCurveNode(k);
			if (!std::strcmp(node->name, "T")) {
				Animation->KeyPositions.push_back(std::vector<Math::Vector3>());
				int i = 0;

				if (node->getCurve(i)) {

					if (Animation->KeyPosCount == 0)
						Animation->KeyPosCount = node->getCurve(i)->getKeyCount();


					for (int p = 0; p < node->getCurve(i)->getKeyCount(); p++) {
						Math::Vector3 Position;
						for (i = 0; i < 3; i++) {
							Position[i] = node->getCurve(i)->getKeyValue()[p];
						}
						i = 0;
						Animation->KeyPositions.back().push_back(Position);
						Animation->KeyPositions.back().back().Print();
					}
				}
			}
			else if (!std::strcmp(node->name, "R")) {
				Animation->KeyRotations.push_back(std::vector<Math::Quaternion>());
				int i = 0;
				if (node->getCurve(i)) {


					if (Animation->KeyRotCount == 0)
						Animation->KeyRotCount = node->getCurve(i)->getKeyCount();

					for (int p = 0; p < node->getCurve(i)->getKeyCount(); p++) {
						Math::Vector3 Rotation;
						for (i = 0; i < 3; i++) {
							Rotation[i] = node->getCurve(i)->getKeyValue()[p];
						}
						i = 0;
						Animation->KeyRotations.back().push_back(Rotation.ToQuaternion());
						Animation->KeyRotations.back().back().Print();
					}
				}
			}

		}
		Application.GetResourceManager()->Add(Animation->GetPath(), Animation);
	}

}
