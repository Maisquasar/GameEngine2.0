#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <ImGui/imgui.h>
#include "Include/Math/Math.h"
namespace Resources
{
	class Texture;
}

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

			virtual void UpdateTransform() {}

			// Function to Draw Picking.
			virtual void DrawPicking(int id) {}

			// Call On Begin Play.
			virtual void Start() {}
			// Call On Creation.
			virtual void Initialize() {}

			virtual void ShowInInspector() { }

			virtual void SetGameobject(Core::Node* node);
			virtual void SetUIIcon() {}

			virtual void Save(std::string space, std::string& content) {}
			virtual void Load(const char* data, uint32_t& pos) { }

			virtual Component* Clone() const = 0;
			// ===== Other Functions ===== //
			// Remove Component from GameObject.
			void RemoveFromParent();

			// ===== Getters Functions ===== //
			bool* GetEnable() { return &_enable; }
			bool IsEnable() { return _enable; }

			Resources::Texture* GetUIIcon() { return _UIIcon; }
		protected:
			bool _enable = true;

			Resources::Texture* _UIIcon = nullptr;

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