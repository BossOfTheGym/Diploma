#include "SystemManager.h"

#include "../Utility/Counter.h"

namespace ecs::sys
{
	SystemManager::SystemManager(ecs::ECSEngine* engine)
		: m_engine(engine)
	{
		// TODO : assert
		//ASSERT(engine && "line: " #LINE "; File: SystemManager.h; engine is nullptr");

		auto sysCount = ecs::util::Counter<Id, ISystem>::get();

		m_dependencyMat.resize(sysCount);
		for (auto& line : m_dependencyMat)
		{
			line.resize(sysCount);
			for(auto& dep : line)
			{
				dep = false;
			}
		}
	}

	ecs::ECSEngine* SystemManager::getECSEngine() const
	{
		return m_engine;
	}
}