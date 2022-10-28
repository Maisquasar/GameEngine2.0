#pragma once
#include "IResource.h"
namespace Resources
{
	class Model : public IResource
	{
	public:
		Model();
		~Model();

		void Load(std::string filename) override;

	private:

	};
}