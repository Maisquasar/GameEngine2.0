#include "Include/Utils/Utils.h"
#include "Include/Core/Node.h"

float Utils::GetDistanceAxis(Math::Vector3 point1, Math::Vector3 point2, Math::Vector3 axis)
{
	// Project the points onto the custom axis
	float dot1 = point1.DotProduct(axis);
	float dot2 = point2.DotProduct(axis);

	// Calculate the distance between the projected points
	return abs(dot1 - dot2);
}

void Utils::SortByDistanceFromCamera(std::vector<Core::Node*>& list, Math::Vector3 CameraPosition, Math::Vector3 Forward)
{
	std::map<Core::Node*, float> distances;
	for (auto element : list)
	{
		distances[element] = GetDistanceAxis(element->Transform.GetWorldPosition(), CameraPosition, Forward);
	}
	auto pairList = BubbleSort(distances);
	list.clear();
	for (auto i : pairList)
	{
		list.push_back(i.first);
	}
}

bool cmp(std::pair<Core::Node*, float>& a, std::pair<Core::Node*, float>& b)
{
	return a.second > b.second;
}

std::vector<std::pair<Core::Node*, float> > Utils::BubbleSort(std::map<Core::Node*, float>& list)
{// Declare vector of pairs
	std::vector<std::pair<Core::Node*, float> > A;

	// Copy key-value pair from Map
	// to vector of pairs
	for (auto& it : list) {
		A.push_back(it);
	}

	// Sort using comparator function
	sort(A.begin(), A.end(), cmp);
	return A;
}
