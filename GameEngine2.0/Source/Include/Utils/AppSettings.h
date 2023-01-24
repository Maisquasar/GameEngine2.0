#pragma once
namespace Utils {

	namespace Settings
	{
		enum class Transform
		{
			Local,
			World
		};
	}

	class AppSettings
	{
	public:
		AppSettings();
		~AppSettings();

		Settings::Transform S_Transform = Settings::Transform::World;
		bool ShowCollider = true;
		size_t NumberOfDrawCalls;
		size_t NumberOfTrianglesDraw;
		size_t NumberOfInstances;
		size_t NumberOfInstancedTriangles;
	private:
		
	};
}