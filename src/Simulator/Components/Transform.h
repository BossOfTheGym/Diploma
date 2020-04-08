#pragma once

#include <Math/MathLib.h>

namespace comp
{
	using math::Vec3f32;
	using math::Quatf32;

	struct Transform
	{
		Vec3f32 translate{};
		Quatf32 rotation{};
		Vec3f32 scale{};
	};
}