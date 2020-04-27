#pragma once

#include <imgui.h>

#include <ECS/ecs_engine.h>
#include <ECS/Entity/Entity.h>

#include <Utility/PureType.h>

#include <utility>
#include <memory>


namespace sim
{
	using ecs::entity::Entity;
	using ecs::entity::EntityComponentRegistry;


	class ISystemInfo : public ecs::util::NoCopyMove
	{
	public:
		ISystemInfo(ecs::sys::SystemManager* manager);

		virtual ~ISystemInfo() = default;


	public:
		virtual void render() = 0;

		virtual const char* name() const = 0;


	public:
		ecs::sys::SystemManager* getSystemManager() const;


	private:
		ecs::sys::SystemManager* m_manager{nullptr};
	};

	template<class SystemT, class SystemInfoClassT>
	class SystemInfo : public ISystemInfo
	{
	public:
		using this_t = SystemInfo;
		using base_t = ISystemInfo;

		using System          = SystemT;
		using SystemInfoClass = SystemInfoClassT;

	public:
		SystemInfo(ecs::sys::SystemManager* manager) 
			: base_t(manager)
		{}
	};


	class IComponentInfo : public ecs::util::NoCopyMove
	{
	public:
		IComponentInfo(ecs::sys::SystemManager* manager);

		virtual ~IComponentInfo() = default;


	public:
		virtual bool hasComponent(Entity e) const = 0;

		virtual void render(Entity e) = 0;

		virtual const char* name() const = 0;


	public:
		ecs::sys::SystemManager* getSystemManager() const;


	public:
		ecs::sys::SystemManager* m_manager{nullptr};
	};

	template<class ComponentT, class ComponentInfoClassT>
	class ComponentInfo : public IComponentInfo
	{
	public:
		using Component          = ComponentT;
		using ComponentInfoClass = ComponentInfoClassT; 

		using this_t = ComponentInfo;
		using base_t = IComponentInfo;


	public:
		ComponentInfo(ecs::sys::SystemManager* manager)
			: base_t(manager)
		{
			// TODO : assert
		}


	public:
		virtual bool hasComponent(Entity e) const override
		{
			auto* sysManager = getSystemManager();
			auto* engine     = sysManager->getECSEngine();
			auto& registry   = engine->getRegistry();

			return registry.valid(e) && registry.has<Component>(e);
		}
	};
}