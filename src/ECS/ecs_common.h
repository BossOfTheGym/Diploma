#pragma once

#include <Math/MathLib.h>

namespace ecs
{
	using Id = math::UInt64;	

	constexpr Id BAD_ID = static_cast<Id>(-1);

	using math::Float;
}