#pragma once

#include <Math/MathLib.h>

#include <Numerics/Arg/ArgN.h>

namespace comp
{
	using math::Float;
	using math::Vec3;

	using StateVec = Num::Arg::VecN<Float, 6>;

	// stores packed radius and velocity
	struct SimData
	{
		StateVec state{};

		void setRadius(const Vec3& r);
		void setVelocity(const Vec3& v);

		Vec3 getRadius() const;
		Vec3 getVelocity() const;
	};

	StateVec fromRadVel(const Vec3& r, const Vec3& v);
}