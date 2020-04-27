#pragma once

#include <ECS/System/System.h>
#include <ECS/Entity/Entity.h>

#include "GuiInfo/GuiInfo.h"

#include <unordered_map> 
#include <memory>


namespace sim
{
	class ImGuiSystem : public ecs::sys::System<ImGuiSystem>
	{
	public:
		using this_t = ImGuiSystem;
		using base_t = ecs::sys::System<ImGuiSystem>;


	private:
		using SystemInfoRegistry    = std::unordered_map<ecs::Id, std::unique_ptr<ISystemInfo>>;
		using ComponentInfoRegistry = std::unordered_map<ecs::Id, std::unique_ptr<IComponentInfo>>;


	public:
		ImGuiSystem(ecs::sys::SystemManager* manager);

		virtual ~ImGuiSystem();


	public: // System<T>
		virtual void update(ecs::Time t, ecs::Time dt) override;


	public:
		template<class ComponentInfoT, class ... Args>
		void registerComponent(Args ... args)
		{
			m_compInfoRegistry[entt::type_info<typename ComponentInfoT::Component>::id()] 
				= std::make_unique<ComponentInfoT>(getSystemManager(), std::forward<Args>(args)...);
		}

		template<class SystemInfoT, class ... Args>
		void registerSystem(Args ... args)
		{
			m_sysInfoRegistry[SystemInfoT::System::TYPE_ID] 
				= std::make_unique<SystemInfoT>(getSystemManager(), std::forward<Args>(args)...);
		}


		void registerEntity(Entity e) const;

		void unregisterEntity(Entity e) const;


	private:
		SystemInfoRegistry    m_sysInfoRegistry{};
		ComponentInfoRegistry m_compInfoRegistry{};
	};
}