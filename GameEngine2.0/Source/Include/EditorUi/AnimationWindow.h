#pragma once
#include "EditorWindow.h"

#include <ImGui/ImGuizmo/ImSequencer.h>
#include <ImGui/ImGuizmo/ImCurveEdit.h>
#include <ImGui/imgui_internal.h>

#include <algorithm>
#include <vector>

#include "Include/Math/Math.h"

namespace Resources
{
	class Animation;
	class Skeleton;
}

namespace Core::Components {
	class AnimationComponent;
}

static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

struct RampEdit : public ImCurveEdit::Delegate
{
	RampEdit()
	{
		mPts.resize(3);

		mbVisible[0] = mbVisible[1] = mbVisible[2] = true;
		mMax = ImVec2(1.f, 1.f);
		mMin = ImVec2(0.f, 0.f);
	}
	size_t GetCurveCount()
	{
		return 3;
	}

	bool IsVisible(size_t curveIndex)
	{
		return mbVisible[curveIndex];
	}
	size_t GetPointCount(size_t curveIndex)
	{
		return mPts[curveIndex].size();
	}

	uint32_t GetCurveColor(size_t curveIndex)
	{
		uint32_t cols[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
		return cols[curveIndex];
	}
	ImVec2* GetPoints(size_t curveIndex)
	{
		return mPts[curveIndex].data();
	}
	virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const { return ImCurveEdit::CurveDiscrete; }
	virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value)
	{
		/*
		mPts[curveIndex][pointIndex] = ImVec2(value.x, value.y);
		SortValues(curveIndex);
		for (size_t i = 0; i < GetPointCount(curveIndex); i++)
		{
			if (mPts[curveIndex][i].x == value.x)
				return (int)i;
		}
		ImGui::Text("Test %d", pointIndex);
		*/
		return pointIndex;
	}
	
	virtual void AddPoint(size_t curveIndex, ImVec2 value)
	{
		mPts[curveIndex].push_back(value);
		SortValues(curveIndex);
	}
	virtual ImVec2& GetMax() { return mMax; }
	virtual ImVec2& GetMin() { return mMin; }
	virtual unsigned int GetBackgroundColor() { return 0; }
	std::vector<std::vector<ImVec2>> mPts;
	bool mbVisible[3];
	ImVec2 mMin;
	ImVec2 mMax;
private:
	void SortValues(size_t curveIndex)
	{
		auto b = std::begin(mPts[curveIndex]);
		auto e = std::begin(mPts[curveIndex]) + GetPointCount(curveIndex);
		std::sort(b, e, [](ImVec2 a, ImVec2 b) { return a.x < b.x; });

	}
};

static const char* SequencerItemTypeNames[] = { "Animation" };

struct MySequenceItem
{
	int mType;
	int mFrameStart, mFrameEnd;
	bool mExpanded;
	const char* name;
};

struct MySequence : public ImSequencer::SequenceInterface
{
	// interface with sequencer

	std::vector<MySequenceItem> myItems;

	virtual int GetFrameMin() const {
		return mFrameMin;
	}
	virtual int GetFrameMax() const {
		return mFrameMax;
	}
	virtual int GetItemCount() const { return (int)myItems.size(); }

	virtual int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
	virtual const char* GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
	virtual const char* GetItemLabel(int index) const
	{
		static char tmps[512];
		snprintf(tmps, 512, "[%02d] %s", index, myItems[index].name);
		return tmps;
	}

	virtual void Get(int index, int** start, int** end, int* type, unsigned int* color)
	{
		MySequenceItem& item = myItems[index];
		if (color)
			*color = 0xFFAA8080; // same color for everyone, return color based on type
		if (start)
			*start = &item.mFrameStart;
		if (end)
			*end = &item.mFrameEnd;
		if (type)
			*type = item.mType;
	}
	virtual void Add(int type) { myItems.push_back(MySequenceItem{ type, 0, 10, false }); };
	virtual void Del(int index) { myItems.erase(myItems.begin() + index); }
	virtual void Duplicate(int index) { myItems.push_back(myItems[index]); }

	virtual size_t GetCustomHeight(int index) { return myItems[index].mExpanded ? 300 : 0; }

	// my datas
	MySequence() : mFrameMin(0), mFrameMax(0) {}
	int mFrameMin, mFrameMax;

	std::vector<RampEdit> rampEdits;

	virtual void DoubleClick(int index) {
		if (myItems[index].mExpanded)
		{
			myItems[index].mExpanded = false;
			return;
		}
		for (auto& item : myItems)
			item.mExpanded = false;
		myItems[index].mExpanded = !myItems[index].mExpanded;
	}

	virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect);

	virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect);
};

namespace EditorUi
{


	class AnimationWindow : public EditorWindow
	{
	public:
		AnimationWindow();
		~AnimationWindow();

		void SetAnimationAndSkeleton(Core::Components::AnimationComponent*, Resources::Skeleton*);
		void Draw() override;

		MySequence Interface;
		Core::Components::AnimationComponent* SelectedAnimationComp = nullptr;
		Resources::Skeleton* SelectedSkeleton = nullptr;

		Math::Vector3* SelectedPosition;
		Math::Quaternion* SelectedRotation;

	private:

	};
}