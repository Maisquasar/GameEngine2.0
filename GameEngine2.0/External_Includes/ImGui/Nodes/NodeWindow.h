#pragma once
#include <ImGui/imgui.h>
#include "Include/Math/Math.h"
#include <vector>

namespace ImGui {
	// ================================ PARAMETERS ================================ //
	enum class ParameterType
	{
		Connect,
		Int,
		Float,
	};

	class Parameter
	{
	public:
		Parameter();
		Parameter(ParameterType T);
		~Parameter();

		void Draw(ImDrawList* draw_list, ImVec2 Pos);

		ParameterType Type = ParameterType::Connect;
	private:

	};

	// ================================ NODE ================================ //
	enum class NodeType
	{
		Variable,
		Function,
		Event,
	};

	class NodeWindow;
	class Node
	{
	public:
		Node();
		Node(NodeType type, ImVec2 size);
		~Node();

		void Draw(ImDrawList* draw_list, ImVec2 origin, Node*& rightclicked);

		std::vector<Parameter> Parameters;

		ImVec2 Position;
		ImVec2 Size;

		ImColor Color;

		NodeType Type = NodeType::Event;

		NodeWindow* Window = nullptr;
	private:

	};

	// ================================ WINDOW ================================ //
	class NodeWindow
	{
	public:
		NodeWindow();
		~NodeWindow();

		void Draw();
		void DrawCanvas();
		void DrawSelection();
		void DrawAllNodes();

		void AddNode(NodeType, ImVec2);

		ImVec2 GetOrigin() { return _origin; }

		Node* MoveNode;
	private:
		bool _open = true;
		bool _isHovered = false;

		ImVec2 _mousePosInCanvas;
		ImVec2 _origin;
		ImDrawList* _drawList;

		std::vector<Node*> _nodes;

		Node _selectionRect;

	};
}