#include "NodeWindow.h"
ImVec2 Add(const ImVec2& a, const ImVec2& b)
{
	return { a.x + b.x, a.y + b.y };
}
ImVec2 Sub(const ImVec2& a, const ImVec2& b)
{
	return { a.x - b.x, a.y - b.y };
}

ImGui::Parameter::Parameter()
{
}

ImGui::Parameter::Parameter(ParameterType T)
{
	Type = T;
}

ImGui::Parameter::~Parameter()
{
}

void ImGui::Parameter::Draw(ImDrawList* draw_list, ImVec2 Pos)
{
	switch (Type)
	{
	case ImGui::ParameterType::Connect: {
		//draw_list->AddCircle(Pos, 5, ImColor(0, 0, 255, 255));
		ImVec2 points[] = { ImVec2(Pos.x - 5, Pos.y - 5), ImVec2(Pos.x - 5, Pos.y + 5), ImVec2(Pos.x + 5, Pos.y + 5), ImVec2(Pos.x + 10, Pos.y), ImVec2(Pos.x + 5, Pos.y - 5) , ImVec2(Pos.x - 5, Pos.y - 5) };
		draw_list->AddPolyline(points, 6, ImColor(255, 255, 255, 255), ImDrawFlags_None, 1);
		break;
	}
	case ImGui::ParameterType::Int:
		draw_list->AddCircle(Pos, 5, ImColor(0, 255, 0, 255));
		break;
	case ImGui::ParameterType::Float:
		draw_list->AddCircle(Pos, 5, ImColor(255, 0, 0, 255));
		break;
	default:
		break;
	}
}

ImGui::Node::Node() {}
ImGui::Node::Node(NodeType type, ImVec2 size)
{
	Type = type;
	Size = size;
	switch (type)
	{
	case ImGui::NodeType::Variable:
		Color = ImColor(20, 180, 20, 255);
		Parameters.push_back(Parameter(ParameterType::Int));
		break;
	case ImGui::NodeType::Function:
		Color = ImColor(20, 20, 180, 255);
		Parameters.push_back(Parameter(ParameterType::Float));
		break;
	case ImGui::NodeType::Event:
		Color = ImColor(180, 20, 20, 255);
		Parameters.push_back(Parameter(ParameterType::Connect));
		break;
	default:
		break;
	}
}

ImGui::Node::~Node() {}

bool IsInsideBox(ImVec2 pos, ImVec2 size, ImVec2 a)
{
	return a.x <= pos.x + size.x && a.x >= pos.x && a.y <= pos.y + size.y && a.y >= pos.y;
}

void ImGui::Node::Draw(ImDrawList* draw_list, ImVec2 origin, Node*& rightclicked)
{
	static ImVec2 OffsetMouse;
	auto pos = ImVec2{ origin.x + Position.x, origin.y + Position.y };

	draw_list->AddRectFilled(pos, { pos.x + Size.x, pos.y + Size.y }, ImColor(100, 100, 100, 180), 5.f);
	draw_list->AddRectFilled(pos, { pos.x + Size.x, pos.y + (Size.y * 0.1f) }, Color, 0.f);

	for (auto p : Parameters)
	{
		p.Draw(draw_list, Add(pos, { Size.x - 15,  Size.y * 0.1f + 25.f }));
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && IsInsideBox(pos, Size, ImGui::GetMousePos()))
	{
		rightclicked = this;
	}
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && IsInsideBox(pos, { Size.x, Size.y * 0.1f }, ImGui::GetMousePos()))
	{
		Window->MoveNode = this;
		OffsetMouse = Sub(ImGui::GetMousePos(), pos);
	}
	else if (Window->MoveNode == this)
	{
		Position = Sub(Sub(GetMousePos(), OffsetMouse), Window->GetOrigin());
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			Window->MoveNode = nullptr;
	}
}

ImGui::NodeWindow::NodeWindow()
{
}

ImGui::NodeWindow::~NodeWindow()
{
	for (auto node : _nodes)
		delete node;
}

void ImGui::NodeWindow::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Node Graph", &_open))
	{
		if (!_drawList)
		{
			_drawList = ImGui::GetWindowDrawList();
		}
		DrawCanvas();
		DrawAllNodes();
		DrawSelection();
	}
	ImGui::End();
}

void ImGui::NodeWindow::DrawCanvas()
{
	static ImVector<ImVec2> points;
	static ImVec2 scrolling(0.0f, 0.0f);
	static bool opt_enable_grid = true;
	static bool opt_enable_context_menu = true;

	ImGui::Text("Mouse Position %f,%f", _mousePosInCanvas.x, _mousePosInCanvas.y);

	// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // ReSize canvas to what's available
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	_drawList->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	_drawList->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

	// This will catch our interactions
	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	_isHovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held
	_origin = { canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y }; // Lock scrolled origin
	_mousePosInCanvas = { io.MousePos.x - _origin.x, io.MousePos.y - _origin.y };

	// Pan (we use a zero mouse threshold when there's no context menu)
	// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
	const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
	if (_isHovered && ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
	{
		scrolling.x += io.MouseDelta.x;
		scrolling.y += io.MouseDelta.y;
	}

	// Draw grid + all lines in the canvas
	_drawList->PushClipRect(canvas_p0, canvas_p1, true);
	if (opt_enable_grid)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			_drawList->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			_drawList->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	}
}

void ImGui::NodeWindow::DrawSelection()
{
	if (MoveNode)
		return;
	static bool inSelection = false;
	if (_isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !inSelection)
	{
		inSelection = true;
		_selectionRect.Position = _mousePosInCanvas;
	}
	else if (inSelection)
	{
		_selectionRect.Size = { (_mousePosInCanvas.x - _selectionRect.Position.x),(_mousePosInCanvas.y - _selectionRect.Position.y) };
		if (IsMouseReleased(ImGuiMouseButton_Left)) {
			inSelection = false;
			_selectionRect.Size = ImVec2(0, 0);
		}
	}
	_drawList->AddRectFilled({ _origin.x + _selectionRect.Position.x, _origin.y + _selectionRect.Position.y }, { _origin.x + _selectionRect.Position.x + _selectionRect.Size.x, _origin.y + _selectionRect.Position.y + _selectionRect.Size.y }, ImColor(20, 20, 150, 100));
}

void ImGui::NodeWindow::DrawAllNodes()
{
	static Node* rightClicked = nullptr;
	for (auto Node : _nodes)
	{
		Node->Draw(_drawList, _origin, rightClicked);
	}
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && _isHovered)
	{
		ImGui::OpenPopup("RightClick");
	}
	if (ImGui::BeginPopup("RightClick"))
	{
		if (ImGui::BeginMenu("New Node")) {
			if (ImGui::Button("Event"))
			{
				AddNode(NodeType::Event, ImVec2(200, 150));
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Variable"))
			{
				AddNode(NodeType::Variable, ImVec2(200, 150));
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Function"))
			{
				AddNode(NodeType::Function, ImVec2(200, 150));
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndMenu();
		}
		if (rightClicked)
		{
			ImGui::Separator();
			if (ImGui::Button("Delete"))
			{
				for (int i = 0; i < _nodes.size(); i++)
				{
					if (_nodes[i] == rightClicked)
					{
						_nodes.erase(_nodes.begin() + i);
						rightClicked = nullptr;
						break;
					}
				}
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndPopup();
	}
}

void ImGui::NodeWindow::AddNode(NodeType t, ImVec2 s)
{
	_nodes.push_back(new Node(t, s));
	_nodes.back()->Window = this;
	_nodes.back()->Position = _mousePosInCanvas;
}