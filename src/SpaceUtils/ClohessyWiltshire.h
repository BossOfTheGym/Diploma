#pragma once

#include <Math/MathLib.h>


namespace space_utils::cw
{
	//n - mean motion
	//t - time

	using math::Float;
	using math::Mat3;
	using math::Vec3;

	Mat3 phi_rr(Float t, Float n);

	Mat3 phi_rv(Float t, Float n);

	Mat3 phi_vr(Float t, Float n);

	Mat3 phi_vv(Float t, Float n);


	struct CWsolution
	{
		Vec3 dr{};
		Vec3 dv{};
	};

	CWsolution dr_dv(const Vec3& dr0, const Vec3& dv0, Float t, Float n);
}