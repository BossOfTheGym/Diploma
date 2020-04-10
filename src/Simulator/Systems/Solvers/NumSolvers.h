#pragma once

#include <utility>

#include <Numerics/Ivp/RungeKutta.h>

#include <Function/FunctionHolder.h>
#include <Solver/SolverWrapper.h>


namespace solvers
{
	using func::FunctionHolder;
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

		using Function = FunctionHolder<Vec(const Time&, const Value&)>;
		using Jacobian = FunctionHolder<Mat(const Time&, const Value&)>;

		using Solver = SolverWrapper<Function, Jacobian, Time, Value>;
	};


	template<class Scalar, int SystemOrder, class Tableau, class Eps>
	auto wrapImplicitNumSolver(Tableau&& tableau, Eps&& eps, int iterationsLimit)
	{
		using Vec  = VecN<Scalar, SystemOrder>;
		using Mat  = MatNxM<Scalar, SystemOrder, SystemOrder>;

		using Time  = Scalar;
		using Value = typename solvers_traits<Scalar, SystemOrder>::Vec;

		using Function = typename solvers_traits<Scalar, SystemOrder>::Function;
		using Jacobian = typename solvers_traits<Scalar, SystemOrder>::Jacobian;

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
		using Vec  = typename solvers_traits<Scalar, SystemOrder>::Vec;
		using Mat  = typename solvers_traits<Scalar, SystemOrder>::Mat;

		using Function = typename solvers_traits<Scalar, SystemOrder>::Function;
		using Jacobian = typename solvers_traits<Scalar, SystemOrder>::Jacobian;

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