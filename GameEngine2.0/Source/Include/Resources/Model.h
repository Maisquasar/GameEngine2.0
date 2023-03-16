#pragma once
#include "IResource.h"
#include "Include/Utils/Loader.h"
#include "Include/Core/Node.h"
#include "Include/Resources/Mesh.h"
namespace Resources
{
	class Model : public BaseResource<Model>, public Core::Node
	{
	public:
		Model();
		~Model();

		void Load() override;
		void LoadSingleThread(std::string filename);
		void ModelLoop(const char* data, const int32_t& size);
		void MultiThreadLoad();

		Core::Node* CreateFromModel();

		bool IsInitialized() override;
	private:

	};
}