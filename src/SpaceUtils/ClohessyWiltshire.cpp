#include "ClohessyWiltshire.h"

namespace space_utils::cw
{
	using math::operator "" _FL;

	Mat3 phi_rr(Float t, Float n)
	{
		Float nt = n * t;
		Float cos_nt = cos(nt);
		Float sin_nt = sin(nt);

		return Mat3(
			  (4.0_FL - 3.0_FL * cos_nt), (6.0_FL * (sin_nt - nt)), (0.0_FL)
			, (0.0_FL)                  , (1.0_FL)                , (0.0_FL)
			, (0.0_FL)                  , (0.0_FL)                , (cos_nt)
		);
	}

	Mat3 phi_rv(Float t, Float n)
	{
		Float nt = n * t;
		Float cos_nt = cos(nt);
		Float sin_nt = sin(nt);

		Float ni  = 1.0_FL / n;
		Float ni2 = 2.0_FL * ni;

		return Mat3(
			  (ni * sin_nt)            , (ni2 * (cos_nt - 1.0_FL))             , (0.0_FL)
			, (ni2 * (1.0_FL - cos_nt)), (ni * (4.0_FL * sin_nt - 3.0_FL * nt)), (0.0_FL)
			, (0.0_FL)                 , (0.0_FL)                              , (ni * sin_nt)
		);
	}

	Mat3 phi_vr(Float t, Float n)
	{
		Float nt = n * t;
		Float cos_nt = cos(nt);
		Float sin_nt = sin(nt);

		return Mat3(
			  (3.0_FL * n * sin_nt), (6.0_FL * n * (cos_nt - 1.0_FL)), (0.0_FL)
			, (0.0_FL)             , (0.0_FL)                        , (0.0_FL)
			, (0.0_FL)             , (0.0_FL)                        , (-n * sin_nt)
		);
	}

	Mat3 phi_vv(Float t, Float n)
	{
		Float nt = n * t;
		Float cos_nt = cos(nt);
		Float sin_nt = sin(nt);

		return Mat3(
			  (cos_nt)         , (-2.0_FL * sin_nt)        , (0.0_FL)
			, (2.0_FL * sin_nt), (4.0_FL * cos_nt - 3.0_FL), (0.0_FL)
			, (0.0_FL)         , (0.0_FL)                  , (cos_nt)
		);
	}

	CWsolution dr_dv(const Vec3& dr0, const Vec3& dv0, Float t, Float n)
	{
		return {
			  (phi_rr(t, n) * dr0 + phi_rv(t, n) * dv0)
			, (phi_vr(t, n) * dr0 + phi_vv(t, n) * dv0)
		};
	}
}
