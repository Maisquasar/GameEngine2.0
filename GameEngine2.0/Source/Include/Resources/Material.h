#pragma once
#include "Texture.h"
#include "Include/Math/Math.h"
namespace Resources
{
	class Material : public IResource
	{
	public:
		Material();
		~Material();

	private:
		Resources::Texture* _texture;
		Math::Vector3 _ambient;
		Math::Vector3 _diffuse;
		Math::Vector3 _specular;
		float _shininess;

	};
}