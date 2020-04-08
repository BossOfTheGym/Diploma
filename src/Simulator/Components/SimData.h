#pragma once

#include <Math/MathLib.h>

#include <Numerics/Arg/ArgN.h>

// TODO : little improvement for comfort work with state
namespace comp
{
	using math::Float;
	using math::Vec3;

	using StateVec = Num::Arg::VecN<Float, 6>;

	// stores packed radius and velocity
	struct SimData
	{
		StateVec state{};

		Vec3 radius() const;
		Vec3 velocity() const;
	};
}