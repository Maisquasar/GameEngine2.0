#include <stdio.h>
#include "Include/Math/Vector/Vector2.h"

using namespace Math;

void Vector2::Print()
{
	printf("(%.2f, %.2f)\n", x, y);
}

std::string Vector2::ToString()
{
	return std::to_string(x) + " " + std::to_string(y);
}