#pragma once
#include <vector>
#include <Windows.h>
#include <glad/glad.h>
#include "Include/Math/Math.h"
#include "Include/Resources/IResource.h"

#define POS 0
#define NML 1
#define TUV 2
#define TAN 3

// Used to Count Vertices and draw Calls.
void glDraw(uint32_t model, GLsizei start, GLsizei count);

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
		virtual void Update(Math::Mat4 MVP, bool outline = false);
		virtual void DrawPicking(Math::Mat4 MVP, int id);

		virtual void SetShader(class Shader*);

		unsigned int _VBO = 0;
		unsigned int _VAO = 0;

		std::vector<Math::IVec3> Indices;
		std::vector<Math::Vec3> Positions;
		std::vector<Math::Vec2> TextureUVs;
		std::vector<Math::Vec3> Normals;

		std::vector<float> Vertices;

		std::vector<SubMesh> SubMeshes;

		bool ShouldDrawCall = true;

	private:
		friend class Core::Components::MeshComponent;
	};
}