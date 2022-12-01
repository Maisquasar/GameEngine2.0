#pragma once
#include <string>
namespace Resources {
	class IResource
	{
	public:
		IResource() {}
		virtual ~IResource() {}
		virtual void Load(std::string filename) {}
		virtual IResource* Clone() const = 0;
		virtual void ShowInInspector() {}
		virtual void Initialize() {}

		void SetPath(std::string path) { _path = path; }
		void SetName(std::string name) { _name = name; }

		std::string GetPath() { return _path; }
		std::string GetName() { return _name; }
		virtual bool IsInitialized() { return _initialized; }

		void SetInitialized() { _initialized = true; }

		bool Loaded = false;
	protected:
		std::string _path;
		std::string _name;
		bool _initialized = false;
	};

	template <typename Derived>
	class BaseResource : public IResource {
	public:
		virtual IResource* Clone() const {
			return new Derived(static_cast<Derived const&>(*this));
		}
	};
}