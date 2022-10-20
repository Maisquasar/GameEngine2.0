#pragma once
namespace Resources {
	class IResource
	{
	public:
		IResource() {}
		virtual ~IResource() {}
		virtual void Load(const char* filename) {}

		bool Loaded = false;
	private:

	};
}