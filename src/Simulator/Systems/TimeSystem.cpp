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
		auto t1 = m_clock.now().time_since_epoch();

		m_t0 += m_dt;
		m_dt  = t1 - m_t;
		if (m_dt.count() > 100'000'000)
		{
			m_dt = Time(100'000'000);
		}

		m_t0Warped += m_dtWarped;
		m_dtWarped  = m_dt * m_warp;

		m_t = t1;
	}


	void TimeSystem::resetTime()
	{
		m_t = m_clock.now().time_since_epoch();

		m_t0 = Time(0);
		m_dt = Time(0);

		m_t0Warped = Time(0);
		m_dtWarped = Time(0);		
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


	auto TimeSystem::getTime() const -> Time
	{
		return m_t0Warped;
	}

	auto TimeSystem::getDeltaTime() const -> Time
	{
		return m_dtWarped;
	}


	auto TimeSystem::getRealTime() const -> Time
	{
		return m_t0;
	}

	auto TimeSystem::getRealTimeDelta() const ->Time
	{
		return m_dt;
	}

	void TimeSystem::addTimeEvent(Time time)
	{
		m_timeEvents.push_back(time);
		std::push_heap(m_timeEvents.begin(), m_timeEvents.end(),
			[] (auto& t1, auto& t2)
			{
				return t1 < t2;
			}
		);
	}

	auto TimeSystem::peekTimeEvent() -> Time
	{
		return m_timeEvents.front();
	}

	void TimeSystem::popTimeEvent()
	{
		std::pop_heap(m_timeEvents.begin(), m_timeEvents.end(),
			[] (auto& t1, auto& t2)
			{
				return t1 < t2;
			}
		);
		m_timeEvents.pop_back();
	}

	bool TimeSystem::hasTimeEvents() const
	{
		return !m_timeEvents.empty();
	}
}