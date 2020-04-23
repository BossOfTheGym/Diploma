#include "PlanetSystem.h"

#include <ECS/Entity/Entity.h>
#include <ECS/System/SystemManager.h>


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

		m_gravitation.reset(new Gravitation());
		m_gravitationJacobian.reset(new GravitationJacobian());

		m_graviHolder         = Function(m_gravitation);
		m_graviJacobianHolder = Jacobian(m_gravitationJacobian);

		m_solver = solvers::gaussLegendre6<Float, 6>(1e-15, 50);
	}

	void PlanetSystem::update(ecs::Time t, ecs::Time dt)
	{
		auto& registry = m_simulator->getRegistry();

		auto planet = m_simulator->getPlanet();
		if (!registry.valid(planet) || !registry.has<Planet, SimData>(planet))
		{
			// TODO : notify about error
			return;
		}

		auto center = registry.get<SimData>(planet).getRadius();
		auto mu     = registry.get<Planet>(planet).mu;
		m_gravitation->setMu(mu);
		m_gravitationJacobian->setMu(mu);

		auto view = registry.view<PhysicsData, SimData>(entt::exclude<Planet>);
		auto tSec  = ecs::toSeconds<Float>(t).count();
		auto dtSec = ecs::toSeconds<Float>(dt).count();
		for (auto& e : view)
		{
			auto& simData = registry.get<SimData>(e);
			auto r = simData.getRadius() - center;
			simData.setRadius(r);

			auto newState = m_solver.solve(m_graviHolder, m_graviJacobianHolder, tSec, simData.state, dtSec);
			simData.state = newState;

			auto& physics = registry.get<PhysicsData>(e);
			physics.r = simData.getRadius();
			physics.v = simData.getVelocity();
		}
	}
}