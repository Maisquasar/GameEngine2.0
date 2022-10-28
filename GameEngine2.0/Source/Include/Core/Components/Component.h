#pragma once
#include <string>
#include <vector>

namespace Core {
	class Node;
	namespace Components {
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

		class Data
		{
		public:
			Data();
			~Data();
			static std::vector<Component> Components;
			static void NewComponent(Component comp);
			void Initialize();
		private:

		};
	}
}