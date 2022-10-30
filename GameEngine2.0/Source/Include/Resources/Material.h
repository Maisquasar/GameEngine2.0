#pragma once
#include "Texture.h"
#include "Include/Math/Math.h"
namespace Resources
{
	class Shader;
	class Material : public IResource
	{
	public:
		Material();
		~Material();
		friend class Model;

		Resources::Shader* GetShader() { return _shader; }
		Resources::Texture* GetTexture() { return _texture; }
		Math::Vector4 GetAmbient() { return _ambient; }
		Math::Vector4 GetDiffuse() { return _diffuse; }
		Math::Vector4 GetSpecular() { return _specular; }
		float GetShiniess() { return _shininess; }
	private:
		Resources::Shader* _shader = nullptr;
		Resources::Texture* _texture = nullptr;
		Math::Vector4 _ambient = Math::Vector4(0);
		Math::Vector4 _diffuse = Math::Vector4(1);
		Math::Vector4 _specular = Math::Vector4(0);
		float _shininess = 32.f;

	};
}