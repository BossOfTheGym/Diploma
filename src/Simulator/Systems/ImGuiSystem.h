#pragma once

#include <ECS/System/System.h>
#include <ECS/Entity/Entity.h>

#include "GuiInfo/GuiInfo.h"

#include <unordered_map> 


namespace sim
{
	class ImGuiSystem : public ecs::sys::System<ImGuiSystem>
	{
	public:
		using this_t = ImGuiSystem;
		using base_t = ecs::sys::System<ImGuiSystem>;


	private:
		using SystemInfoRegistry    = std::unordered_map<ecs::Id, SystemInfo>;
		using ComponentInfoRegistry = std::unordered_map<ecs::Id, ComponentInfo>;


	public:
		ImGuiSystem(ecs::sys::SystemManager* manager);

		virtual ~ImGuiSystem();


	public: // System<T>
		virtual void update(ecs::Float t, ecs::Float dt) override;


	public:
		template<class Component, class ComponentInfoT>
		void registerComponent(ComponentInfoT&& info)
		{
			m_compInfoRegistry[entt::type_info<Component>::id()] = ComponentInfo(std::forward<ComponentInfo>(info), TypeTag<Component>);
		}

		template<class System, class SystemInfoT>
		void registerSystem(SystemInfoT&& info)
		{
			m_sysInfoRegistry[System::TYPE_ID] = SystemInfo(std::forward<SystemInfoT>(info), TypeTag<System>);
		}


	private:
		SystemInfoRegistry    m_sysInfoRegistry{};
		ComponentInfoRegistry m_compInfoRegistry{};
	};
}