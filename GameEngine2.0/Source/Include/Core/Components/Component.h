#pragma once
#include <string>

namespace Core {
	class Node;
	namespace Component {
		class Component
		{
		public:
			Component();
			~Component();

			virtual void Update() {}
			virtual void EditorUpdate() {}
			virtual void GameUpdate() {}

			// Call On Begin Play;
			virtual void Start() {}
			// Call On Creation.
			virtual void Initialize() {}

			virtual void ShowInInspector() {}

			Core::Node* GameObject = nullptr;
			std::string ComponentName = "Empty";

		private:

		};

	}
}