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

		// time system is never updated in normal way
		disable();
	}


	void TimeSystem::tick()
	{
		auto t1 = m_clock.now().time_since_epoch().count();

		m_t0 += m_dt;
		m_dt  = t1 - m_t;

		m_t0Warped += m_dtWarped;
		m_dtWarped  = m_dt * m_warp;

		m_t = t1;
	}


	void TimeSystem::resetTime()
	{
		m_t = m_clock.now().time_since_epoch().count();

		m_t0 = 0;
		m_dt = 0;

		m_t0Warped = 0;
		m_dtWarped = 0;		
	}

	void TimeSystem::setWarp(Tick warp)
	{
		m_warp = std::min(std::max(warp, m_minWarp), m_maxWarp);
	}


	auto TimeSystem::getWarp() const -> Tick
	{
		return m_warp;
	}

	auto TimeSystem::getMinWarp() const -> Tick
	{
		return m_minWarp;
	}

	auto TimeSystem::getMaxWarp() const -> Tick
	{
		return m_maxWarp;
	}


	auto TimeSystem::getTime() const -> Tick
	{
		return m_t0Warped;
	}

	auto TimeSystem::getDeltaTime() const -> Tick
	{
		return m_dtWarped;
	}


	auto TimeSystem::getRealTime() const -> Tick
	{
		return m_t0;
	}

	auto TimeSystem::getRealTimeDelta() const ->Tick
	{
		return m_dt;
	}

	void TimeSystem::addTimeEvent(Tick time)
	{
		m_ticks.push_back(time);
		std::push_heap(m_ticks.begin(), m_ticks.end(), 
			[] (auto& t1, auto& t2)
			{
				return t1 < t2;
			}
		);
	}

	auto TimeSystem::peekTimeEvent() -> std::optional<Tick>
	{
		if (hasTimeEvents())
		{
			return m_ticks.front();
		}
		return std::nullopt;
	}

	void TimeSystem::popTimeEvent()
	{
		std::pop_heap(m_ticks.begin(), m_ticks.end(),
			[] (auto& t1, auto& t2)
			{
				return t1 < t2;
			}
		);
		m_ticks.pop_back();
	}

	bool TimeSystem::hasTimeEvents() const
	{
		return !m_ticks.empty();
	}
}