#include "PlanetSystem.h"

#include <ECS/System/SystemManager.h>

#include "Functions/Functions.h"
#include "Solvers/NumSolvers.h"

#include "../Components/SimData.h"
#include "../Components/PhysicsData.h"
#include "../Components/Planet.h"
#include "../Simulator.h"

namespace sim
{
	using comp::StateVec;
	using comp::SimData;
	using comp::PhysicsData;
	using comp::Planet;
	
	PlanetSystem::PlanetSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		m_simulator = static_cast<Simulator*>(manager->getECSEngine());

	}

	void PlanetSystem::update(ecs::Float t, ecs::Float dt)
	{
		auto& registry = m_simulator->getRegistry();

		auto planetView = registry.view<Planet, SimData>();
		if (planetView.begin() == planetView.end())
		{
			return;
		}
		
		Entity planet = *planetView.begin();
		auto center = registry.get<SimData>(planet).radius();

		auto view = registry.view<PhysicsData, SimData>(entt::exclude<Planet>);
		for (auto& e : view)
		{
			auto& physics = registry.get<PhysicsData>(e);
			auto& simData = registry.get<SimData>(e);
			simData.state[0] -= center[0];
			simData.state[1] -= center[1];
			simData.state[2] -= center[2];

			auto newState = m_solver.solve(m_gravitationFunction, m_gravitationJacobian, t, simData.state, dt);
			simData.state = newState;
			physics.r = simData.radius();
			physics.v = simData.velocity();
		}
	}
}