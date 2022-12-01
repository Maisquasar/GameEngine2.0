#pragma once
#include "Texture.h"
#include "Include/Math/Math.h"
namespace Resources
{
	class Shader;
	class Material : public BaseResource<Material>
	{
	public:
		Material();
		~Material();

		void ShowInInspector() override;
		void Load(std::string filename) override;
		void MultiThreadLoad(std::string filename);

		// Ptrs Getters
		Math::Vector4* GetPtrAmbient() { return &_ambient; }
		Math::Vector4* GetPtrDiffuse() { return &_diffuse; }
		Math::Vector4* GetPtrSpecular() { return &_specular; }
		float* GetPtrShiniess() { return &_shininess; }

		// Getters
		Resources::Shader* GetShader() { return _shader; }
		Resources::Texture* GetTexture() { return _texture; }
		Math::Vector4 GetAmbient() { return _ambient; }
		Math::Vector4 GetDiffuse() { return _diffuse; }
		Math::Vector4 GetSpecular() { return _specular; }
		float GetShiniess() { return _shininess; }
		bool IsEditable() { return _editable; }

		// Setters
		void SetShader(Resources::Shader* shader) { _shader = shader; }
		void SetTexture(Resources::Texture* texture) { _texture = texture; }
		void SetAmbient(Math::Vector4 ambient) { _ambient = ambient; }
		void SetDiffuse(Math::Vector4 diffuse) { _diffuse = diffuse; }
		void SetSpecular(Math::Vector4 specular) { _specular = specular; }
		void SetShiniess(float shininess) { _shininess = shininess; }
		void SetTransparency(float transparency) {
			_ambient.w = transparency;
			_diffuse.w = transparency;
			_specular.w = transparency;
		}
		void SetEditable(bool editable) { _editable = editable; }
	private:
		friend class Mesh;
		friend class Model;

		Resources::Shader* _shader = nullptr;
		Resources::Texture* _texture = nullptr;
		Math::Vector4 _ambient = Math::Vector4(0);
		Math::Vector4 _diffuse = Math::Vector4(1);
		Math::Vector4 _specular = Math::Vector4(0);
		float _shininess = 32.f;

		bool _editable = true;

	};
}