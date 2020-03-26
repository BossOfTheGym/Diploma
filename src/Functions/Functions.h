#pragma once

#include <Numerics/Arg/ArgN.h>
#include <Math/MathLib.h>

#include "../Function/FunctionWrapper.h"

namespace funcs
{
	template<class Scalar, int Dim>
	struct func_traits
	{
		using Type = std::remove_reference_t<std::remove_cv_t<Scalar>>;

		using Vec = Num::Arg::VecN<Type, Dim>;
		using Mat = Num::Arg::MatNxM<Type, Dim, Dim>;

		using Time  = Type;
		using Value = Vec;

		using Function = func::FunctionWrapper<Vec(const Time&, const Value&)>;
		using Jacobian = func::FunctionWrapper<Mat(const Time&, const Value&)>;
	};

	// returns tuple of gravitation function and its jacobian
	template<class Scalar, int Order>
	auto gravitationForce(Scalar&& planetMu)
	{
		// equation of motiion is the second order equation
		using traits = func_traits<Scalar, 2 * Order>;

		using Type = typename traits::Type;

		using Time  = typename traits::Time;
		using Value = typename traits::Value;

		using Mat = typename traits::Mat;
		using Vec = typename traits::Vec;

		using Function = typename traits::Function;
		using Jacobian = typename traits::Jacobian;

		// u: packed radius and velocity vectors, (r, v)
		auto equ = [mu = planetMu] (const Time& t, const Value& u)
		{
			Value res{};
			for (int i = 0; i < Order; i++)
			{
				res[i] = u[Order + i];
			}

			Type sumSquares{};
			for (int i = 0; i < Order; i++)
			{
				sumSquares += u[i] * u[i];
			}

			Type mult = -mu / std::pow(sumSquares, Type(3.0 / 2.0));
			for (int i = 0; i < Order; i++)
			{
				res[Order + i] = mult * u[i];
			}

			return res;
		};

		auto jac = [mu = planetMu] (const Time& t, const Value& u)
		{
			auto d = [] (auto i, auto j)
			{
				return (i == j ? 1 : 0);
			};

			Mat res{};
			for (int i = 0; i < Order; i++)
			{
				res[i][Order + i] = Type(1.0);
			}

			Type sumSquares{};
			for (int i = 0; i < Order; i++)
			{
				sumSquares += u[i] * u[i];
			}

			Type mult = -mu / std::pow(sumSquares, Type(3.0 / 2.0));
			for (int i = 0; i < Order; i++)
			{
				for (int j = 0; j < Order; j++)
				{
					res[Order + i][j] = mult * (d(i, j) - 3 * u[i] * u[j] / sumSquares);
				}
			}

			return res;
		};

		return std::make_tuple(Function(equ), Jacobian(jac));
	}


	//	auto jacob = [&] (const Time& t, const Value& u)
	//	{
	//		
	//	};

	//	return 0;
	//}

	//// returns tuple of perturbed gravitation function and its jacobian 
	//template<
	//	  class Scalar
	//	, int Order
	//	, class Perturbation
	//	, class PerturbationJacobian
	//>
	//auto gravitatationForcePerturbed(
	//	  Scalar&& planetMu
	//	, Perturbation&& perturbation
	//	, PerturbationJacobian&& perturbationJacobian
	//)
	//{
	//	return 0;
	//}
}