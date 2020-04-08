#include "TimeSystem.h"

#include <algorithm>

namespace sim
{
	TimeSystem::TimeSystem(ecs::sys::SystemManager* manager, Tick warp, Tick minWarp, Tick maxWarp)
		: base_t(manager)
	{
		m_minWarp = std::min(minWarp, maxWarp);
		m_maxWarp = std::max(minWarp, maxWarp);
		
		setWarp(warp);
		resetTime();
	}


	void TimeSystem::tick()
	{
		auto t1 = m_clock.now().time_since_epoch().count();

		m_t0 += m_dt;
		m_dt  = t1 - m_t0;

		m_t0Warped += m_dtWarped;
		m_dtWarped  = m_dt * m_warp;
	}


	void TimeSystem::resetTime()
	{
		m_t = m_clock.now();

		m_t0 = 0;
		m_dt = 0;

		m_t0Warped = 0;
		m_dtWarped = 0;		
	}

	void TimeSystem::setWarp(Tick warp)
	{
		m_warp = std::min(std::max(warp, m_minWarp), m_maxWarp);
	}


	TimeSystem::Tick TimeSystem::getWarp() const
	{
		return m_warp;
	}

	TimeSystem::Tick TimeSystem::getMinWarp() const
	{
		return m_minWarp;
	}

	TimeSystem::Tick TimeSystem::getMaxWarp() const
	{
		return m_maxWarp;
	}


	TimeSystem::Tick TimeSystem::getTime() const
	{
		return m_t0Warped;
	}

	TimeSystem::Tick TimeSystem::getDeltaTime() const
	{
		return m_dtWarped;
	}
}