#pragma once

#include <utility>

#include <Numerics/Ivp/RungeKutta.h>

#include "../Function/FunctionWrapper.h"
#include "../Solver/SolverWrapper.h"


namespace solvers
{
	using func::FunctionWrapper;
	using solver::SolverWrapper;

	using Num::Arg::VecN;
	using Num::Arg::MatNxM;


	template<class Scalar, int SystemOrder>
	struct solvers_traits
	{
		using Vec  = VecN<Scalar, SystemOrder>;
		using Mat  = MatNxM<Scalar, SystemOrder, SystemOrder>;

		using Time  = Scalar;
		using Value = Vec;

		using Function = FunctionWrapper<Vec(const Time&, const Value&)>;
		using Jacobian = FunctionWrapper<Mat(const Time&, const Value&)>;
	};


	template<class Scalar, int SystemOrder, class Tableau, class Eps>
	auto wrapImplicitNumSolver(Tableau&& tableau, Eps&& eps, int iterationsLimit)
	{
		using Vec  = VecN<Scalar, SystemOrder>;
		using Mat  = MatNxM<Scalar, SystemOrder, SystemOrder>;

		using Time  = Scalar;
		using Value = Vec;

		using Function = FunctionWrapper<Vec(const Scalar&, const Vec&)>;
		using Jacobian = FunctionWrapper<Mat(const Scalar&, const Vec&)>;

		return SolverWrapper<Function, Jacobian, Time, Value>(
			Num::Ivp::make_rki_solver<SystemOrder, Time, Value>(
				  std::forward<Tableau>(tableau)
				, std::forward<Eps>(eps)
				, iterationsLimit
			)
		);
	}


	template<class Scalar, int SystemOrder, class Eps>
	auto implicitMidpoint2(Eps&& eps, int iterationsLimit)
	{
		return wrapImplicitNumSolver<Scalar, SystemOrder>(
			  Num::Ivp::ImplicitMethods<Scalar>::midpoint2()
			, std::forward<Eps>(eps)
			, iterationsLimit
		);
	}

	template<class Scalar, int SystemOrder, class Eps>
	auto gaussLegendre4(Eps&& eps, int iterationsLimit)
	{
		return wrapImplicitNumSolver<Scalar, SystemOrder>(
			Num::Ivp::ImplicitMethods<Scalar>::gaussLegendre4()
			, std::forward<Eps>(eps)
			, iterationsLimit
		);
	}

	template<class Scalar, int SystemOrder, class Eps>
	auto gaussLegendre6(Eps&& eps, int iterationsLimit)
	{
		return wrapImplicitNumSolver<Scalar, SystemOrder>(
			  Num::Ivp::ImplicitMethods<Scalar>::gaussLegendre6()
			, std::forward<Eps>(eps)
			, iterationsLimit
		);
	}



	template<class Scalar, int SystemOrder, class Tableau>
	auto wrapExplicitNumSolver(Tableau&& tableau)
	{
		using Time = Scalar;
		using Vec  = VecN<Scalar, SystemOrder>;
		using Mat  = MatNxM<Scalar, SystemOrder, SystemOrder>;

		using Function = FunctionWrapper<Vec(const Scalar&, const Vec&)>;
		using Jacobian = FunctionWrapper<Mat(const Scalar&, const Vec&)>;

		return SolverWrapper<Function, Jacobian, Time, Vec>(
			Num::Ivp::make_rkeg_solver<SystemOrder, Scalar, Vec>(
				  std::forward<Tableau>(tableau)
			)
		);
	}


	template<class Scalar, int SystemOrder>
	auto classic4()
	{
		return wrapExplicitNumSolver<Scalar, SystemOrder>(
			Num::Ivp::ExplicitMethods<Scalar>::classic4()
		);
	}
}