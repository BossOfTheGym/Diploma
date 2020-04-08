#include "ISystem.h"

#include <cassert>

namespace ecs::sys
{
	ISystem::ISystem(SystemManager* manager, Id staticTypeId)
		: m_manager(manager)
		, ecs::util::StaticType(staticTypeId)
	{
		// TODO : asserts
		//assert((manager && "Line: " ##__LINE__ "; file: " __FILE__ "; manager is nullptr"));
		//assert((staticTypeId == ecs::INVALID && "Line: " __LINE__ "; file: " __FILE__ "; staticTypeId is INVALID"));
	}

	void ISystem::disable()
	{
		m_enabled = false;
	}

	void ISystem::enable()
	{
		m_enabled = true;
	}

	bool ISystem::enabled() const
	{
		return m_enabled;
	}


	SystemManager* ISystem::getSystemManager() const
	{
		return m_manager;
	}
}