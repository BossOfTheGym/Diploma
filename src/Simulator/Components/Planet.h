#pragma once

#include <Math/MathLib.h>

namespace comp
{
	using math::Float;

	struct Planet
	{
		Float mu{};
		Float R{};
		Float w{};
		Float g0{};
		Float J2{};
	};
}
