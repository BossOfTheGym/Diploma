#include "ImGuiSystem.h"

#include <ECS/System/SystemManager.h>

#include <glWrap/OpenGL.h>
#include <glWrap/Framebuffer/Framebuffer.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ContextSystem.h"

namespace sim
{
	ImGuiSystem::ImGuiSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		// TODO : assert
		auto* contextSys = manager->get<ContextSystem>();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(contextSys->getHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 430 core");
	}

	ImGuiSystem::~ImGuiSystem()
	{
		// TODO
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiSystem::update(ecs::Float t, ecs::Float dt)
	{
		// TODO
		// TEST
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static bool show_demo_window = true;
			static bool show_another_window = false;
			static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);


			if (show_another_window)
			{
				ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;
				ImGui::End();
			}
		}

		ImGui::Render();
		gl::Framebuffer::getDefaultFramebuffer().clear(gl::ClearMask::Depth);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}