#pragma once

#include <Math/MathLib.h>

namespace comp
{
	using math::Vec3;
	using math::Quat;
	using math::Float;

	struct PhysicsData
	{
		Vec3 r{};
		Vec3 v{};

		Vec3 axis{};
		Float angle{}; // rad per second

		Float mass{};
	};
}