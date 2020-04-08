#pragma once

#include <Math/MathLib.h>

#include "StateVec.h"

namespace space_utils
{
	//n - mean motion
	//t - time
	Mat3 phi_rr(Float t, Float n);

	Mat3 phi_rv(Float t, Float n);

	Mat3 phi_vr(Float t, Float n);

	Mat3 phi_vv(Float t, Float n);


	StateVec dr_dv(const Vec3& dr0, const Vec3& dv0, Float t, Float n);
}