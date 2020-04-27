#include "ImGuiSystem.h"

#include <ECS/System/SystemManager.h>

#include <glWrap/OpenGL.h>
#include <glWrap/Framebuffer/Framebuffer.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../Components/ImGuiTag.h"
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
		ImGui_ImplOpenGL3_Init("#version 450 core");
	}

	ImGuiSystem::~ImGuiSystem()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiSystem::update(ecs::Time t, ecs::Time dt)
	{
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
		// END TEST

		ImGui::Begin("Editor");

		// framerate
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Separator();

		// systems
		if (ImGui::CollapsingHeader("Systems parameters"))
		{
			for (auto& [sysID, sysInfo] : m_sysInfoRegistry)
			{
				ImGui::PushID(sysID);
				ImGui::PushID("SystemInfo");
				sysInfo->render();
				ImGui::PopID();
				ImGui::PopID();
			}
		}

		// components
		if (ImGui::CollapsingHeader("Entity editor"))
		{ 
			auto& registry = getSystemManager()->getECSEngine()->getRegistry();

			for (auto e : registry.view<comp::ImGuiTag>())
			{
				ImGui::PushID(entt::to_integral(e));
				ImGui::Text("%u", entt::to_integral(e));
				ImGui::Indent(10.0f);
				for (auto& [compID, componentInfo] : m_compInfoRegistry)
				{
					if (componentInfo->hasComponent(e))
					{
						ImGui::PushID(compID);
						ImGui::PushID("ComponentInfo");
						componentInfo->render(e);
						ImGui::PopID();
						ImGui::PopID();
					}
				}
				ImGui::Unindent(10.0f);
				ImGui::PopID();
			}
		}
		ImGui::End();

		ImGui::Render();

		gl::Framebuffer::getDefaultFramebuffer().clear(gl::ClearMask::Depth);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiSystem::registerEntity(Entity e) const
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
		if (registry.valid(e) && !registry.has<comp::ImGuiTag>(e))
		{
			registry.assign<comp::ImGuiTag>(e);
		}
	}

	void ImGuiSystem::unregisterEntity(Entity e) const
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
		if (registry.valid(e))
		{
			registry.remove_if_exists<comp::ImGuiTag>(e);
		}
	}
}