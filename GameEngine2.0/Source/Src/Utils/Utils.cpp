#include "Include/Utils/Utils.h"
#include "Include/Core/Node.h"
#include <ImGui/imgui.h>

float Utils::GetDistanceAxis(Math::Vec3 point1, Math::Vec3 point2, Math::Vec3 axis)
{
	// Project the points onto the custom axis
	float dot1 = point1.DotProduct(axis);
	float dot2 = point2.DotProduct(axis);

	// Calculate the distance between the projected points
	return abs(dot1 - dot2);
}

void Utils::SortByDistanceFromCamera(std::vector<Core::Node*>& list, Math::Vec3 CameraPosition, Math::Vec3 Forward)
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

void Utils::GetPlaneVertices(std::vector<Math::Vec3>& Positions, std::vector<Math::Vec2>& TextureUVs, std::vector<Math::Vec3>& Normals, std::vector<Math::IVec3>& Indices)
{
	std::vector<Math::Vec3> v = {
		{1.000000f, 1.000000f, 0.000000},
		{ 1.000000, -1.000000, 0.000000 },
		{-1.000000, 1.000000, 0.000000},
		{-1.000000, -1.000000, 0.000000 },
	};

	std::vector < Math::Vec2> t = {
		{1 - 0.000000, 0.000000},
		{1 - 1.000000, 0.000000 },
		{1 - 0.000000, 1.000000},
		{1 - 1.000000, 1.000000 }
	};
	Math::Vec3 n = { -0.0000, 1.0000, -0.0000 };

	std::vector<Math::IVec3> i{
		{1, 1, 0}, {2, 2, 0}, {0, 0, 0},
		{1, 1, 0}, {3, 3, 0}, {2, 2, 0}
	};

	Positions.insert(Positions.begin(), v.begin(), v.end());
	TextureUVs.insert(TextureUVs.begin(), t.begin(), t.end());
	Normals.push_back(n);
	Indices.insert(Indices.begin(), i.begin(), i.end());
}

float Utils::RandomFloat(float min, float max)
{
	static std::mt19937 gen(std::random_device{}());
	if (min > max)
		return min;
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}

struct InputTextCompletion {
	int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }

	std::vector<std::string> texts;

	int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{

		// Example of TEXT COMPLETION

		// Locate beginning of current word
		const char* word_end = data->Buf + data->CursorPos;
		const char* word_start = word_end;
		while (word_start > data->Buf)
		{
			const char c = word_start[-1];
			if (c == ' ' || c == '\t' || c == ',' || c == ';')
				break;
			word_start--;
		}

		// Build a list of candidates
		ImVector<const char*> candidates;
		for (int i = 0; i < texts.size(); i++)
			if (Strnicmp(texts[i].c_str(), word_start, (int)(word_end - word_start)) == 0)
				candidates.push_back(texts[i].c_str());

		if (candidates.Size == 1)
		{
			// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
			data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
			data->InsertChars(data->CursorPos, candidates[0]);
			data->InsertChars(data->CursorPos, " ");
		}
		else
		{
			// Multiple matches. Complete as much as we can..
			// So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
			int match_len = (int)(word_end - word_start);
			for (;;)
			{
				int c = 0;
				bool all_candidates_matches = true;
				for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
					if (i == 0)
						c = toupper(candidates[i][match_len]);
					else if (c == 0 || c != toupper(candidates[i][match_len]))
						all_candidates_matches = false;
				if (!all_candidates_matches)
					break;
				match_len++;
			}

			if (match_len > 0)
			{
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
			}
		}


		return 0;
	}
};

static int TextEditCallback(ImGuiInputTextCallbackData* data)
{
	InputTextCompletion* inputText = (InputTextCompletion*)data->UserData;
	return inputText->TextEditCallbackStub(data);
}


bool Utils::InputTextAutoCompletion(std::vector<std::string> datas, std::string& result)
{
	InputTextCompletion it;
	it.texts.insert(it.texts.begin(), datas.begin(), datas.end());

	static char out[256];
	strcpy_s(out, 256, result.c_str());
	bool value = ImGui::InputText("Test", out, 256, ImGuiInputTextFlags_CallbackCompletion, TextEditCallback, &it);
	//ImGui::SameLine();
	static bool isOpen = false;
	bool isFocused = ImGui::IsItemFocused();
	isOpen |= ImGui::IsItemActive();
	if (isOpen && ImGui::IsItemEdited())
	{
		ImGui::SetNextWindowPos({ ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y });
		ImGui::SetNextWindowSize({ ImGui::GetItemRectSize().x, 0 });
		if (ImGui::Begin("##popup", &isOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Tooltip))
		{
			ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());
			isFocused |= ImGui::IsWindowFocused();
			for (int i = 0; i < datas.size(); i++)
			{
				if (strstr(datas[i].c_str(), out) == NULL)
					continue;
				if (ImGui::Selectable(datas[i].c_str()) || (ImGui::IsItemFocused() && ImGui::IsKeyPressedMap(ImGuiKey_Enter)))
				{
					strcpy_s(out, datas[i].c_str());
					isOpen = false;
				}
			}
		}
		ImGui::End();
		isOpen &= isFocused;
	}
	result = out;
	return value;
}
