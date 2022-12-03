#include "Include/Utils/Utils.h"
#include "Include/Core/Node.h"

void Utils::SortByDistanceFromCamera(std::vector<Core::Node*>& list, Math::Vector3 CameraPosition)
{
	std::map<Core::Node*, float> distances;
	for (auto element : list)
	{
		distances[element] = (element->Transform.GetWorldPosition().GetDistanceBetween(CameraPosition));
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
