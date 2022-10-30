#pragma once
#include "IResource.h"
#include "Include/Utils/Loader.h"
#include "Include/Core/Node.h"
#include "Include/Core/Components/Mesh.h"
namespace Resources
{
	class Model : public IResource, public Core::Node
	{
	public:
		Model();
		~Model();

		void Load(std::string filename) override;
		void ModelLoop(const char* data, const int32_t& size);
		void MaterialLoop(std::string path);

	private:

	};
}