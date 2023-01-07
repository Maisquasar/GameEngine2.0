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
		size_t NumberOfDrawCalls;
		size_t NumberOfTrianglesDraw;
	private:
		
	};
}