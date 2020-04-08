#pragma once

#include <ECS/Entity/Entity.h>
#include <ECS/System/System.h>

#include <Math/MathLib.h>

#include "Solvers/NumSolvers.h"
#include "Functions/Functions.h"

#include <map>
#include <string>


namespace sim
{
	using solver_traits = solvers::solvers_traits<math::Float, 2 * 3>;
	using Function = typename solver_traits::Function;
	using Jacobian = typename solver_traits::Jacobian;
	using Solver   = typename solver_traits::Solver;

	class Simulator;

	class PlanetSystem final : public ecs::sys::System<PlanetSystem>
	{
	public:
		using this_t = PlanetSystem;
		using base_t = ecs::sys::System<PlanetSystem>;

		using Entity = ecs::entity::Entity;

		using Name = std::string;
		using Id   = ecs::Id;

		using Solvers   = std::map<Id, Solver>;
		using Functions = std::map<Id, Function>;

		using NameId = std::map<Name, Id>;
		using IdName = std::map<Id, Name>;

		//static const Id BAD_ID;
		//static const Name BAD_NAME;

	public:
		PlanetSystem(ecs::sys::SystemManager* manager);

		virtual ~PlanetSystem() = default;


	public:
		virtual void update(ecs::Float t, ecs::Float dt);


	public:
		//bool loadSolver();
		// TODO : add ability to load solvers

	private:
		Simulator* m_simulator{nullptr};

		Solver   m_solver{};
		Function m_gravitationFunction{};
		Jacobian m_gravitationJacobian{};
	};
}