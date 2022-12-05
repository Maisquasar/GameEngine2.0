#pragma once
#include "../Debug/Log.h"
#include <map>
#include <glad/glad.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <string>

#include "IResource.h"

namespace Resources
{
	enum class Location
	{
		L_MVP,
		L_M,
		L_TIME,
		L_ENABLE_TEXTURE,
		L_TEXTURE,
		L_MATERIAL,
		L_NORMAL_MAP,
		L_DIRECTIONAL,
		L_POINT,
		L_SPOT,
		L_COLOR,
	};

	class Shader : public BaseResource<Shader>
	{
	public:
		Shader();
		~Shader();
		int Program = -1;
		int VertexShader = 0;
		int FragmentShader = 0;

		// Set Vertex Shader.S
		bool SetShader(std::string filename);
		// Set Fragment Shader.
		bool SetFragmentShader(std::string filename);

		void Load(std::string filename) override;

		void Recompile();

		// Link shader and set if is a screenShader.
		bool Link();
		// Initialize the Shader
		void Initialize();
		// Reload Locations.
		int GetLocation(Location loc);

	private:
		bool _linked = false;
		std::map<Location, int> _location;
	};
}