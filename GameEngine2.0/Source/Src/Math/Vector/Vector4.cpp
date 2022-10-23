#include "Include/Math/Vector/Vector4.h"

void Math::Vector4::Print()
{
	printf("(%.2f, %.2f, %.2f, %.2f)\n", x, y, z, w);
}

std::string Math::Vector4::ToString()
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w);
}