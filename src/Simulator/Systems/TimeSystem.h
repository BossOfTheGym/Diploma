#pragma once

#include <Math/MathLib.h>

#include <ECS/System/System.h>

#include <chrono>
#include <vector>
#include <optional>
#include <algorithm>

namespace sim
{
	class TimeSystem : public ecs::sys::System<TimeSystem>
	{
	public:
		using this_t = TimeSystem;
		using base_t = ecs::sys::System<TimeSystem>;

		using Clock = ecs::Clock;
		using Time  = ecs::Time;
		using Tick  = ecs::Tick;

		using Heap = std::vector<Time>;


	public:
		TimeSystem(ecs::sys::SystemManager* manager, Tick warp = 1, Tick minWarp = 1, Tick maxWarp = 1);


	public:
		void tick();

		void resetTime();

		void setWarp(Tick warp);


		Tick getWarp() const;

		Tick getMinWarp() const;

		Tick getMaxWarp() const;


		Time getTime() const;

		Time getDeltaTime() const;


		Time getRealTime() const;

		Time getRealTimeDelta() const;


		void addTimeEvent(Time time);

		Time peekTimeEvent(); // alwaus check for present events

		void popTimeEvent();

		bool hasTimeEvents() const;


		bool stopped();

		void stop();

		void resume();


	private:
		Clock m_clock{};

		Tick m_warp{1};    // warp coefficient
		Tick m_minWarp{1}; // min warp 
		Tick m_maxWarp{1}; // max warp

		Time m_t{};
		
		Time m_t0{}; // current real time
		Time m_dt{}; // real time delta

		Time m_t0Warped{}; // warped current time
		Time m_dtWarped{}; // warped delta time

		Heap m_timeEvents{}; // used for producing correct time deltas

		bool m_stopped{false};
	};
}