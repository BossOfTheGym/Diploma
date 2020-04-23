#pragma once

#include <ECS/Entity/Entity.h>
#include <ECS/System/System.h>

#include <Math/MathLib.h>

#include <map>
#include <string>
#include <memory>

#include "Solvers/NumSolvers.h"
#include "Functions/Functions.h"


namespace sim
{
	using solver_traits = solvers::solvers_traits<math::Float, 2 * 3>;
	using Function = typename solver_traits::Function;
	using Jacobian = typename solver_traits::Jacobian;
	using Solver   = typename solver_traits::Solver;
	

	using math::Float;

	using Gravitation         = funcs::Gravitation<Float, 3>;
	using GravitationJacobian = funcs::GravitationJacobian<Float, 3>;
	using GravitationPtr         = std::shared_ptr<Gravitation>;
	using GravitationJacobianPtr = std::shared_ptr<GravitationJacobian>;



	class Simulator;

	class PlanetSystem final : public ecs::sys::System<PlanetSystem>
	{
	public:
		using this_t = PlanetSystem;
		using base_t = ecs::sys::System<PlanetSystem>;


	public:
		PlanetSystem(ecs::sys::SystemManager* manager);

		virtual ~PlanetSystem() = default;


	public:
		virtual void update(ecs::Time t, ecs::Time dt);


	private:
		Simulator* m_simulator{nullptr};

		Solver   m_solver{};
		GravitationPtr         m_gravitation{};
		GravitationJacobianPtr m_gravitationJacobian{};
		Function m_graviHolder{};
		Jacobian m_graviJacobianHolder{};
	};
}