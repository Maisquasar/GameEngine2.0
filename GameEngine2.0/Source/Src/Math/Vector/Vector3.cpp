#include "Include/Math/Vector/Vector3.h"

void Math::Vector3::Print() const
{
	printf("(%.2f, %.2f, %.2f)\n", x, y, z);
}
std::string Math::Vector3::ToString()
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}
