#include "..\..\Include\EditorUi\AnimationWindow.h"
#include "Include/Resources/Animation.h"
#include "Include/Core/Components/AnimationComponent.h"
#include "Include/Resources/Skeleton.h"
#include "Include/App.h"
#include <ImGui/ImGuizmo/ImSequencer.h>

void MySequence::CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
{
	static const char* labels[] = { "Translation", "Rotation" , "Scale" };

	rampEdits[index].mMax = ImVec2(float(mFrameMax), 1.f);
	rampEdits[index].mMin = ImVec2(float(mFrameMin), 0.f);
	draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
	for (int i = 0; i < 3; i++)
	{
		ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
		ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
		draw_list->AddText(pta, rampEdits[index].mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
		if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
			rampEdits[index].mbVisible[i] = !rampEdits[index].mbVisible[i];
	}
	draw_list->PopClipRect();

	ImGui::SetCursorScreenPos(rc.Min);
	ImVector<ImCurveEdit::EditPoint> editPoints;
	ImCurveEdit::Edit(rampEdits[index], rc.Max - rc.Min, 137 + index, &clippingRect, &editPoints);
	ImGui::SetCursorScreenPos(ImVec2(rc.Min.x + 10, rc.Min.y + 10));
	if (editPoints.size() >= 1) {
		if (editPoints[0].curveIndex == 0) {
			int i = editPoints[0].pointIndex;
			auto keypos = &EditorUi::Editor::GetAnimationWindow()->SelectedAnimationComp->GetCurrentAnimation()->KeyPositions;
			if (keypos->size() > index && keypos[index].size() > i) {
				EditorUi::Editor::GetAnimationWindow()->SelectedPosition = &(*keypos)[index][i];
				EditorUi::Editor::GetAnimationWindow()->SelectedRotation = nullptr;
			}
		}
		else
		{
			int i = editPoints[0].pointIndex;
			auto keyrot = &EditorUi::Editor::GetAnimationWindow()->SelectedAnimationComp->GetCurrentAnimation()->KeyRotations;
			if (keyrot->size() > index && keyrot[index].size() > i) {
				EditorUi::Editor::GetAnimationWindow()->SelectedRotation = &(*keyrot)[index][i];
				EditorUi::Editor::GetAnimationWindow()->SelectedPosition = nullptr;
			}
		}
	}
}

void MySequence::CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
{
	rampEdits[index].mMax = ImVec2(float(mFrameMax), 1.f);
	rampEdits[index].mMin = ImVec2(float(mFrameMin), 0.f);
	draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
	/*for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < rampEdit.mPts[i].size(); j++)
		{
			float p = rampEdit.mPts[i][j].x;
			if (p < myItems[index].mFrameStart || p > myItems[index].mFrameEnd)
				continue;
			float r = (p - mFrameMin) / float(mFrameMax - mFrameMin);
			float x = ImLerp(rc.Min.x, rc.Max.x, r);
			draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
		}
	}*/
	draw_list->PopClipRect();
}

EditorUi::AnimationWindow::AnimationWindow()
{
	_open = false;
	Interface.mFrameMin = 0;
	Interface.mFrameMax = 100;
}

EditorUi::AnimationWindow::~AnimationWindow()
{
}

void EditorUi::AnimationWindow::SetAnimationAndSkeleton(Core::Components::AnimationComponent* a, Resources::Skeleton* s)
{
	if (!a || !a->GetCurrentAnimation() || !s)
		return;
	SelectedAnimationComp = a;
	SelectedSkeleton = s;
	Interface.myItems.clear();
	for (int i = 0; i < s->BoneCount; i++) {
		Interface.myItems.push_back(MySequenceItem{ 0, 0, (int)a->GetCurrentAnimation()->KeyCount, false, s->Bones[i]->Name.c_str() });
		Interface.rampEdits.push_back(RampEdit());
		if (a->GetCurrentAnimation()->KeyPositions.size() > i) {
			for (int j = 0; j < a->GetCurrentAnimation()->KeyPositions[i].size(); j++)
			{
				Interface.rampEdits.back().AddPoint(0, ImVec2((float)j, 0.75f));
			}
		}
		if (a->GetCurrentAnimation()->KeyRotations.size() > i) {
			for (int j = 0; j < a->GetCurrentAnimation()->KeyRotations[i].size(); j++)
			{
				Interface.rampEdits.back().AddPoint(1, ImVec2((float)j, 0.5f));
			}
		}
			//Interface.Add(0);
	}
}

void EditorUi::AnimationWindow::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Animation", &_open))
	{
		// let's create the sequencer
		static int selectedEntry = -1;
		static int firstFrame = 0;
		static bool expanded = true;
		bool play = SelectedAnimationComp ? SelectedAnimationComp->_play : false;
		static int currentFrame = 0;

		ImGui::PushItemWidth(130);
		ImGui::InputInt("Frame Min", &Interface.mFrameMin);

		ImGui::SameLine();
		ImGui::InputInt("Frame Max", &Interface.mFrameMax);

		ImGui::SameLine();
		if (ImGui::Button("<|<|")) currentFrame = 0;
		ImGui::SameLine();
		if (ImGui::Button("|<|")) currentFrame--;
		ImGui::SameLine();
		if (ImGui::Button("|>")) play = !play;
		ImGui::SameLine();
		if (ImGui::Button("|>|")) currentFrame++;
		ImGui::SameLine();
		if (ImGui::Button("|>|>")) currentFrame = Interface.mFrameMax;
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(200);
		if (SelectedPosition)
		{
			ImGui::SameLine();
			Math::Vector3 value = *SelectedPosition;
			ImGui::DragFloat3("Position", &value.x, 0.05f, 0.0F, 0.0F, "%.5f");
			*SelectedPosition = value;
		}
		else if (SelectedRotation)
		{
			ImGui::SameLine();
			Math::Quaternion value = *SelectedRotation;
			ImGui::DragFloat4("Rotation", &value.x, 0.05f, 0.0F, 0.0F, "%.5f");
			*SelectedRotation = value;
		}
		ImGui::PopItemWidth();

		Sequencer(&Interface, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_CHANGE_FRAME);
		// add a UI to edit that particular item
		if (selectedEntry != -1)
		{
			const MySequenceItem& item = Interface.myItems[selectedEntry];
			ImGui::Text("I am a %s, please edit me", SequencerItemTypeNames[item.mType]);
			// switch (type) ....
		}
		if (SelectedAnimationComp && SelectedAnimationComp->GetCurrentAnimation()) {
			SelectedAnimationComp->_play = play;
			if (!play) {
				SelectedAnimationComp->_currentTime = (float)currentFrame;
				SelectedAnimationComp->_currentTime = (float)((int)SelectedAnimationComp->_currentTime % (Interface.mFrameMax + 1));
				if (SelectedAnimationComp->_currentTime < 0)
					SelectedAnimationComp->_currentTime = (float)SelectedAnimationComp->GetCurrentAnimation()->KeyCount;
			}
			currentFrame = (int)SelectedAnimationComp->_currentTime;
		}
	}

	ImGui::End();
}
