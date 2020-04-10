#pragma once

#include <Math/MathLib.h>

namespace comp
{
	using Tick = math::UInt64;

	struct Timer
	{
		Tick duration{};
	};
}