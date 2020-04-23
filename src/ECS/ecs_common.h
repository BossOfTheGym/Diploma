#pragma once

#include <Math/MathLib.h>

#include <chrono>

namespace ecs
{
	using Id = math::UInt64;	

	constexpr Id BAD_ID = static_cast<Id>(-1);

	using Clock = std::chrono::steady_clock;
	using Time  = std::chrono::nanoseconds;
	using Tick  = Time::rep;

	template<class Type>
	using Seconds = std::chrono::duration<Type>;

	template<class Ty, class TimeValue>
	auto toSeconds(TimeValue t)
	{
		return std::chrono::duration_cast<Seconds<Ty>>(t);
	}
}