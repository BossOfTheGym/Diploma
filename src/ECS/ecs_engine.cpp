#include "ecs_engine.h"

namespace ecs
{
	ECSEngine::ECSEngine() 
		: m_systemManager(this)
		, m_entityBuilderManager(this)
	{}

	auto ECSEngine::getRegistry() -> Registry&
	{
		return m_registry;
	}

	auto ECSEngine::getSystemManager() -> SystemManager&
	{
		return m_systemManager;
	}

	auto ECSEngine::getEntityBuilderManager() ->EntityBuilderManager&
	{
		return m_entityBuilderManager;
	}


	void ECSEngine::mainLoop()
	{
		// do nothing
	}
}