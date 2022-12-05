#pragma once
#include <string>
#include <vector>
#include "Include/Math/Math.h"
#include "ImGui/imgui.h"

namespace Core {
	class Node;
	namespace Components {
		class Component
		{
		public:
			Core::Node* GameObject = nullptr;
			std::string ComponentName = "Empty";

			Component();
			virtual ~Component() = default;

			// ===== Virtual Functions ===== //
			virtual void Update() {}
			virtual void EditorUpdate() {}
			virtual void GameUpdate() {}

			// Call On Begin Play.
			virtual void Start() {}
			// Call On Creation.
			virtual void Initialize() {}

			virtual void ShowInInspector() { }

			virtual void SetGameobject(Core::Node* node);

			virtual void Save(std::string space, std::string& content) {}
			virtual void Load(const char* data, uint32_t& pos) { }

			virtual Component* Clone() const = 0;
			// ===== Other Functions ===== //
			// Remove Component from GameObject.
			void RemoveFromParent();

			// ===== Getters Functions ===== //
			bool* GetEnable() { return &_enable; }
			bool IsEnable() { return _enable; }
		protected:
			bool _enable = true;

		};

		template <typename Derived>
		class BaseComponent : public Component {
		public:
			typedef Component Super;
			virtual Component* Clone() const {
				return new Derived(static_cast<Derived const&>(*this));
			}
		};

		class Data
		{
		public:
			Data();
			~Data();
			static void Destroy();
			static std::vector<Component*> Components;
			static void NewComponent(Component* comp);
			void Initialize();
		private:

		};
	}
}