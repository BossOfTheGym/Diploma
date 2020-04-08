#pragma once

#include "ecs_common.h"

#include "Entity/Entity.h"
#include "Entity/EntityBuilderManager.h"
#include "System/SystemManager.h"

#include "Utility/NoCopyMove.h"

namespace ecs
{
	class ECSEngine : public ecs::util::NoCopyMove
	{
	public:
		using Registry             = ecs::entity::EntityComponentRegistry;
		using SystemManager        = ecs::sys::SystemManager;
		using EntityBuilderManager = ecs::entity::EntityBuilderManager;


	public:
		ECSEngine();

		virtual ~ECSEngine() = default;


	public:
		Registry& getRegistry();

		SystemManager& getSystemManager();

		EntityBuilderManager& getEntityBuilderManager();


	public:
		virtual void mainLoop();


	protected:
		Registry             m_registry;
		SystemManager        m_systemManager;
		EntityBuilderManager m_entityBuilderManager;
	};
}