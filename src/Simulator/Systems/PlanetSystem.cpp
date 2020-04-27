#include "PlanetSystem.h"

#include <ECS/ecs_engine.h>
#include <ECS/Entity/Entity.h>
#include <ECS/System/SystemManager.h>


#include "../Components/SimData.h"
#include "../Components/PhysicsData.h"
#include "../Components/Transform.h"
#include "../Components/Planet.h"

#include "SimulatorState.h"
#include "PhysicsSystem.h"

namespace sim
{
	using comp::StateVec;
	using comp::SimData;
	using comp::PhysicsData;
	using comp::Planet;
	
	using math::operator "" _FL;

	PlanetSystem::PlanetSystem(ecs::sys::SystemManager* manager, ecs::Time dt, ecs::Tick maxUpdates)
		: base_t(manager)
		, m_dt{dt}
		, m_maxUpdates{maxUpdates}
	{
		m_maxUpdates = (m_maxUpdates != 0 ? m_maxUpdates : 1);

		m_gravitation.reset(new Gravitation());
		m_gravitationJacobian.reset(new GravitationJacobian());

		m_graviHolder         = Function(m_gravitation);
		m_graviJacobianHolder = Jacobian(m_gravitationJacobian);
		
		m_solver = solvers::classic4<Float, 6>();
	}

	void PlanetSystem::update(ecs::Time t, ecs::Time dt)
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();

		auto* simulatorState = getSystemManager()->get<SimulatorState>();

		auto planet = simulatorState->getPlanet();
		if (!registry.valid(planet) || !registry.has<Planet, SimData>(planet))
		{
			// TODO : notify about error
			return;
		}

		auto center = registry.get<SimData>(planet).getRadius();
		auto mu     = registry.get<Planet>(planet).mu;
		m_gravitation->setMu(mu);
		m_gravitationJacobian->setMu(mu);

		auto updates = dt / m_dt;
		auto step = m_dt;
		if (updates > m_maxUpdates)
		{
			step = dt / m_maxUpdates;
			updates = m_maxUpdates;
		}
		auto stepSec = ecs::toSeconds<Float>(step).count();

		for (auto e : registry.view<PhysicsData, SimData>(entt::exclude<Planet>))
		{
			auto& simData = registry.get<SimData>(e);
			auto r = simData.getRadius() - center;
			simData.setRadius(r);

			auto state = simData.state;
			for (int i = 1; i < updates; i++)
			{
				auto tSec  = ecs::toSeconds<Float>(t).count();
				state = m_solver.solve(m_graviHolder, m_graviJacobianHolder, tSec, state, stepSec);

				t  += step;
				dt -= step;
			}
			auto tSec  = ecs::toSeconds<Float>(t).count();
			auto dtSec = ecs::toSeconds<Float>(dt).count();
			simData.state = m_solver.solve(m_graviHolder, m_graviJacobianHolder, tSec, state, dtSec);

			auto& physics = registry.get<PhysicsData>(e);
			physics.r = simData.getRadius();
			physics.v = simData.getVelocity();
		}
	}

	void PlanetSystem::updateEntity(Entity e)
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
		if (registry.valid(e) && registry.has<comp::PhysicsData, comp::SimData>(e))
		{
			auto& simData = registry.get<comp::SimData>(e);

			if (registry.has<comp::PhysicsData>(e))
			{
				auto& physicsData = registry.get<comp::PhysicsData>(e);
				physicsData.r = simData.getRadius();
				physicsData.v = simData.getVelocity();
			}
			
			if (registry.has<comp::Transform>(e))
			{
				auto& transform = registry.get<comp::Transform>(e);
				transform.translate = simData.getRadius();
			}
		}
	}
}