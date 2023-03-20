#pragma once
#include <Windows.h>
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
		Line(float lineWidth);
		Line(Math::Vec3 p1, Math::Vec3 p2, float lineWidth = 1);
		~Line();
		
		void Initialize();
		void Draw(Math::Vec3 p1, Math::Vec3 p2);
	private:
		Math::Vec3 _point1;
		Math::Vec3 _point2;
		float _lineWidth = 1.f;

		unsigned int _VAO = 0;
		unsigned int _VBO = 0;
		Resources::Shader* _shader = nullptr;
		bool _initialized = false;

	};
}