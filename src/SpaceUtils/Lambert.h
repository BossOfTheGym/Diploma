#pragma once

#include <Math/MathLib.h>

namespace space_utils
{
	using math::Vec3;
	using math::Float;
	using math::UInt32;

	namespace lambert
	{
		//case 1:
		//if revolutions count = 0 then velocity1 is for hyperbolic trajectory
		//and velocity2 is for elliptical trajectory
		//case 2:
		//both velocities are for elliptical trajectory
		struct Solution
		{
			Vec3 vel1;
			Vec3 vel2;
		};

		Solution solve(
			  const Vec3& rv1
			, Float t1
			, const Vec3& rv2
			, Float t2
			, const Vec3& hv
			, Float mu
			, UInt32 revolutions
			, Float eps = math::EPS
			, int limit = 10
		);
	}
}
