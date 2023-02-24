#pragma once
#define NOMINMAX
#include "Include/Debug/Log.h"

#include <windows.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/ImGuizmo/ImSequencer.h>
#include <ImGui/ImGuizmo/ImGuizmo.h>

#include <OpenFBX/ofbx.h>

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <memory>
#include <fstream>
#include <functional>
#include <map>