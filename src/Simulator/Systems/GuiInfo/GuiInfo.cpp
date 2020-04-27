#include "GuiInfo.h"

namespace sim
{
	ISystemInfo::ISystemInfo(ecs::sys::SystemManager* manager)
		: m_manager(manager)
	{
		// TODO : assert
	}

	ecs::sys::SystemManager* ISystemInfo::getSystemManager() const
	{
		return m_manager;
	}

	IComponentInfo::IComponentInfo(ecs::sys::SystemManager* manager)
		: m_manager(manager)
	{
		// TOOD : assert
	}

	ecs::sys::SystemManager* IComponentInfo::getSystemManager() const
	{
		return m_manager;
	}
}