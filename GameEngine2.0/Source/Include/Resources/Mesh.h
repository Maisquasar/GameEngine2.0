#pragma once
#include "Include/Math/Math.h"
#include "Include/Resources/Material.h"
#include "Include/Resources/IResource.h"
#define POS 0
#define NML 1
#define TUV 2
#define TAN 3

namespace Resources {
	class Mesh : public Resources::IResource
	{
	public:
		Mesh();
		~Mesh();

		void Load(std::string filename) override;
		void VerticesLoop(std::vector<unsigned int>& indices, std::vector<float>& vertices);

		unsigned int _VBO = 0;
		unsigned int _VAO = 0;

		std::vector<Math::Integer3> Indices;
		std::vector<Math::Vector3> Positions;
		std::vector<Math::Vector2> TextureUVs;
		std::vector<Math::Vector3> Normals;

		std::vector<float> Vertices;

		std::vector<Resources::Material*> Materials;

	private:

	};
}