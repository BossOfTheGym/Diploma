#pragma once

#include <memory>
#include <utility>

#include "ISolver.h"


namespace solver
{
	template<class Function, class Jacobian, class Time, class Value, class Solver>
	class SolverHolder : public ISolver<Function, Jacobian, Time, Value>
	{
	public:
		using SolverBase = ISolver<Function, Jacobian, Time, Value>;

		using BaseInterface = ISolver<Function, Jacobian, Time, Value>;
		using Pointer       = typename BaseInterface::Pointer;

	public:
		template<class SolverType>
		SolverHolder(SolverType solver)
			: m_solver(std::forward<SolverType>(solver))
		{}

		SolverHolder() = default;
		SolverHolder(const SolverHolder&) = default;
		SolverHolder(SolverHolder&&)      = default;

		virtual ~SolverHolder() = default;


		template<class SolverType>
		SolverHolder& operator = (SolverType solver)
		{
			m_solver = std::forward<SolverType>(solver);

			return *this;
		}

		SolverHolder& operator = (const SolverHolder& solver) = default;
		SolverHolder& operator = (SolverHolder&& solver)      = default;


	public:
		virtual Value solve(const Function& f, const Jacobian& j, Time t, Value u, Time dt) override
		{
			return m_solver.solve(f, j, t, u, dt);
		}

		[[nodiscard]]
		virtual Pointer copy() const override
		{
			return Pointer(new SolverHolder(*this));
		}


	private:
		Solver m_solver;
	};


	template<class Function, class Jacobian, class Time, class Value, class Solver>
	auto make_solver_holder(Solver&& solver)
	{
		using SolverType = std::remove_cv_t<std::remove_reference_t<Solver>>;

		return SolverHolder<Function, Jacobian, Time, Value, SolverType>(std::forward<Solver>(solver));
	}


	template<class Function, class Jacobian, class Time, class Value, class Solver>
	[[nodiscard]] auto make_solver_holder_ptr(Solver&& solver)
	{
		using SolverType = std::remove_cv_t<std::remove_reference_t<Solver>>;

		return new SolverHolder<Function, Jacobian, Time, Value, SolverType>(std::forward<Solver>(solver));
	}



	template<class Function, class Jacobian, class Time, class Value>
	class SolverWrapper : public ISolver<Function, Jacobian, Time, Value>
	{
	public:
		using SolverBase = ISolver<Function, Jacobian, Time, Value>;
		using SolverPtr  = std::unique_ptr<SolverBase>;

		using BaseInterface = ISolver<Function, Jacobian, Time, Value>;
		using Pointer       = typename BaseInterface::Pointer;


	public:
		template<class Solver>
		SolverWrapper(Solver solver)
			: m_ptr(make_solver_holder_ptr<Function, Jacobian, Time, Value>(std::forward<Solver>(solver)))
		{}


		SolverWrapper(const SolverWrapper& solverWrapper)
			: m_ptr(solverWrapper.m_ptr->copy())
		{}

		SolverWrapper(SolverWrapper&& solverWrapper)
			: m_ptr()
		{
			std::swap(m_ptr, solverWrapper.m_ptr);
		}


		template<class Solver>
		SolverWrapper& operator = (Solver solver)
		{
			m_ptr.reset(make_solver_holder_ptr<Function, Time, Value>(std::forward<Solver>(solver)));
		}

		SolverWrapper& operator = (const SolverWrapper& wrapper)
		{
			m_ptr.reset(wrapper.m_ptr->copy());

			return *this;
		}

		SolverWrapper& operator = (SolverWrapper&& wrapper)
		{
			m_ptr.swap(wrapper.m_ptr);

			return *this;
		}


		SolverWrapper()  = default;
		~SolverWrapper() = default;


	public:
		virtual Value solve(const Function& f, const Jacobian& j, Time t, Value u, Time dt) override
		{
			return m_ptr->solve(f, j, t, u, dt);
		}

		[[nodiscard]]
		virtual Pointer copy() const override
		{
			return Pointer(new SolverWrapper(*this));
		}


	private:
		SolverPtr m_ptr;
	};
}