#pragma once
#include <string>
namespace Resources {
	class IResource
	{
	public:
		IResource() {}
		virtual ~IResource() {}
		virtual void Load(std::string filename) {}

		void SetPath(std::string path) { _path = path; }
		void SetName(std::string name) { _name = name; }

		std::string GetPath() { return _path; }
		bool Loaded = false;
	protected:
		std::string _path;
		std::string _name;
	};
}