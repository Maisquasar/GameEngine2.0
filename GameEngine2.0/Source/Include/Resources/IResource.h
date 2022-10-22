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
		bool Loaded = false;
	protected:
		std::string _path;
	};
}