#pragma once
#include "Include/Math/Math.h"
namespace Resources
{
	class Shader;
}
namespace Debug {
	class Line
	{
	public:
		Line();
		Line(Math::Vector3 p1, Math::Vector3 p2, float lineWidth = 1);
		~Line();
		
		void Initialize();
		void Draw();
	private:
		Math::Vector3 _point1;
		Math::Vector3 _point2;
		float _lineWidth = 1.f;

		unsigned int _VAO;
		unsigned int _VBO;
		Resources::Shader* _shader;

	};
}