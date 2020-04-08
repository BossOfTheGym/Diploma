#pragma once

#include <Math/MathLib.h>

#include <chrono>

#include <ECS/System/System.h>


namespace sim
{
	using math::UInt64;

	class TimeSystem : public ecs::sys::System<TimeSystem>
	{
	public:
		using this_t = TimeSystem;
		using base_t = ecs::sys::System<TimeSystem>;

		using Clock    = std::chrono::steady_clock;
		using Time     = Clock::time_point;
		using Duration = Clock::duration;

		using Tick = UInt64;

		static constexpr Tick PERIOD = Duration::period::den;


	public:
		TimeSystem(ecs::sys::SystemManager* manager, Tick warp = 1, Tick minWarp = 1, Tick maxWarp = 1);


	public:
		void tick();

		void resetTime();

		void setWarp(Tick warp);


		Tick getWarp() const;

		Tick getMinWarp() const;

		Tick getMaxWarp() const;


		Tick getTime() const;

		Tick getDeltaTime() const;


	private:
		Clock m_clock{};

		Tick m_warp{1};    // warp coefficient
		Tick m_minWarp{1}; // min warp 
		Tick m_maxWarp{1}; // max warp

		Time m_t{}; // ground zero

		Tick m_t0{}; // current real time
		Tick m_dt{}; // real time delta

		Tick m_t0Warped{}; // warped current time
		Tick m_dtWarped{}; // warped delta time
	};
}