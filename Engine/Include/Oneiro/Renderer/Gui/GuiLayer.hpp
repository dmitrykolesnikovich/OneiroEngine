//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Core/Logger.hpp"

#include <string>

namespace oe::Renderer::GuiLayer
{
	using namespace ImGui;
	inline void PreInit()
	{
		IMGUI_CHECKVERSION();
		CreateContext();
		StyleColorsDark();
	}

	inline void Init()
	{
		ImGui_ImplGlfw_InitForOpenGL(Core::Root::GetWindow()->GetGLFW(), true);
		std::string glVersion = Core::Root::GetGLVersion();
		ImGui_ImplOpenGL3_Init(("#version " + Core::Root::GetGLSLVersion()).c_str());
	}

	inline void NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	inline void Draw()
	{
		Render();
		ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
	}

	inline void Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		DestroyContext();
	}
}
