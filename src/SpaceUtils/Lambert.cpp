#include "Lambert.h"
#include "Stumpff.h"

#include <Numerics/Equ/Neuton.h>


namespace space_utils
{
	namespace lambert
	{
		//initialD  :D
		struct InitialX
		{
			Float x0;
			Float x1;
		};

		InitialX x_K_revolutions(Float rho, Float sigma, UInt32 k)
		{
			Float um = std::sqrt(1.0_FL - math::SQ2 * std::abs(rho));

			Float eps1 = std::pow(math::PI * k / (math::SQ2 * (sigma - rho * um)), 1.0_FL / 3) * um;
			Float eps2 = std::pow(math::PI * (k + 1) / (2.0_FL / 3 * std::pow(um, 3.0) + sigma - rho * um), 1.0_FL / 3) * um;

			Float z1 = eps1;
			Float z2 = math::PI - eps2;

			return {4 * std::pow(z1 + math::PI * k, 2), 4 * std::pow(z2 + math::PI * k, 2)};
		}

		Float C(Float z)
		{
			if (z > std::numeric_limits<Float>::epsilon())
			{
				return (1.0_FL - std::cos(std::sqrt(z))) / z;
			}
			else if (z < std::numeric_limits<Float>::epsilon())
			{
				return (std::cosh(std::sqrt(-z)) - 1.0_FL) / -z;
			}
			return 1.0_FL / 2;
		}

		Float S(Float z)
		{
			if (z > std::numeric_limits<Float>::epsilon())
			{
				Float zSq  = std::sqrt(z);
				Float zSq3 = z * zSq;

				return (zSq - std::sin(zSq)) / zSq3;

			}
			else if (z < std::numeric_limits<Float>::epsilon())
			{
				Float zSq  = std::sqrt(-z);
				Float zSq3 = -z * zSq;

				return (std::sinh(zSq) - zSq) / zSq3;
			}			
			return 1.0 / 6;
		}


		Solution solve(
			  const Vec3& rv1  // rad-vec 1
			, Float t1        // time1
			, const Vec3& rv2  // rad-vec 2
			, Float t2        // time2
			, const Vec3& hv
			, Float mu        // 
			, UInt32 revolutions  // revolutions on transfer
			, Float eps       // eps for solver
			, int limit        // iterations limit
			)
		{
			using namespace stumpff;

			using Solver = Num::Equ::NeutonScalar<Float, Num::Equ::neu_sc_norm<Float>>;


			Solver solver(eps, limit);

			const Float r1 = glm::length(rv1);
			const Float r2 = glm::length(rv2);

			auto transfer = std::acos(glm::dot(rv1 / r1, rv2 / r2));
			if (glm::dot(glm::cross(rv1, rv2), hv) < 0.0)
			{
				transfer = math::PI_2 - transfer;
			}

			const Float tau = t2 - t1;
			const Float rho   = std::sqrt(2 * r1 * r2) / (r1 + r2) * std::cos(transfer / 2);
			const Float sigma = std::sqrt(mu) / std::pow(r1 + r2, 1.5_FL) * tau;


			// initial guess
			auto[x1, x2] = x_K_revolutions(rho, sigma, revolutions);

			// func
			auto F = [&] (Float x) -> Float
			{
				auto c1Val = c1(x);
				auto c2Val = c2(x);
				auto c3Val = c3(x);

				auto uVal  = std::sqrt(1.0_FL - rho * c1Val / std::sqrt(c2Val));
				auto uVal3 = uVal * uVal * uVal;

				return c3Val / std::pow(c2(x), 1.5_FL) * uVal3 + rho * uVal - sigma;
			};

			// deriv
			auto Fx = [&] (Float x) -> Float
			{
				auto c1Val = c1(x);
				auto c2Val = c2(x);
				auto c3Val = c3(x);
				auto c4Val = c4(x);
				auto c5Val = c5(x);
				auto c6Val = c6(x);

				auto c3Val2 = c3Val * c3Val;

				auto c2Pow = std::pow(c2Val, 1.5);

				auto u = std::sqrt(1.0_FL - rho * c1Val / std::sqrt(c2Val));
				auto u2 = u * u;
				auto u3 = u2 * u;

				return (c3Val2 - c5Val + 4 * c6Val) / (4 * c2Pow) * u3
					+ (3 * c3Val / c2Pow * u2 + rho) * rho * std::sqrt(c2Val) / (8 * u);
			};

			// solution
			auto solution = [&] (Float x0) -> Vec3
			{
				auto x = solver.solve(F, Fx, x0);

				auto c1Val = c1(x);
				auto c2Val = c2(x);
				auto c3Val = c3(x);

				Float u = std::sqrt(1.0_FL - rho * c1Val / std::sqrt(c2Val));
				Float s = std::sqrt((r1 + r2) / (mu * c2Val)) * u;
				Float mus2 = mu * s * s;

				Float f = 1.0_FL - mus2 * c2Val / r1;
				Float g = tau - mus2 * s * c3Val;

				return 1.0_FL / g * (rv2 - f * rv1);
			};

			return {solution(x1), solution(x2)};
		}
	}
}