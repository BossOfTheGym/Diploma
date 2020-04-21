#pragma once

#include <Math/MathLib.h>

namespace space_utils
{
	using math::Float;
	using math::Mat3;
	using math::Vec3;

	struct StateVec
	{
		Vec3 r{};
		Vec3 v{};
	};
}