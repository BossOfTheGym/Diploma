#include "EntityBuilderManager.h"

namespace ecs::entity
{
	EntityBuilderManager::EntityBuilderManager(ecs::ECSEngine* m_engineInstance)
		: m_engine(m_engineInstance)
	{
		// TODO : assert
	}

	ecs::ECSEngine* EntityBuilderManager::getEngineInstance() const
	{
		return m_engine;
	}
}