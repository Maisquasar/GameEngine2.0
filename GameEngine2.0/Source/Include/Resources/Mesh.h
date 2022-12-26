#pragma once
#include <vector>
#include "Include/Math/Math.h"
#include "Include/Resources/IResource.h"
#define POS 0
#define NML 1
#define TUV 2
#define TAN 3

namespace Core::Components
{
	class MeshComponent;
}

namespace Resources {
	struct SubMesh
	{
		class Material* Material;
		size_t StartIndex;
		size_t Count;
	};
	class Mesh : public BaseResource<Mesh>
	{
	public:
		Mesh();
		~Mesh();

		void Load(std::string filename) override;
		virtual void Initialize() override;
		
		void VerticesLoop(std::vector<unsigned int>& indices, std::vector<float>& vertices);

		// Call on MeshComponent Update.
		void Update(Math::Matrix4 MVP, bool outline = false);
		void DrawPicking(Math::Matrix4 MVP, int id);

		unsigned int _VBO = 0;
		unsigned int _VAO = 0;

		std::vector<Math::Integer3> Indices;
		std::vector<Math::Vector3> Positions;
		std::vector<Math::Vector2> TextureUVs;
		std::vector<Math::Vector3> Normals;

		std::vector<float> Vertices;

		std::vector<SubMesh> SubMeshes;

	private:
		friend class Core::Components::MeshComponent;
	};
}