//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"

namespace oe::Renderer
{
	class OE_API GuiLayer
	{
	public:
		static void PreInit()
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			const ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::StyleColorsDark();
		}

		static void Init()
		{
			ImGui_ImplGlfw_InitForOpenGL(Core::Root::GetWindow()->GetGLFW(), true);
			ImGui_ImplOpenGL3_Init("#version 330");
		}

		static void NewFrame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		static void Draw()
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		static void Shutdown()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	};
}
