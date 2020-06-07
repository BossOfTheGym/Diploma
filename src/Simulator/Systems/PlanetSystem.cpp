#include "PlanetSystem.h"

#include <ECS/ecs_engine.h>
#include <ECS/Entity/Entity.h>
#include <ECS/System/SystemManager.h>


#include "../Components/SimData.h"
#include "../Components/PhysicsData.h"
#include "../Components/Orbit.h"
#include "../Components/Transform.h"
#include "../Components/Planet.h"

#include "SimulatorState.h"
#include "PhysicsSystem.h"

#include <iostream>

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

		// TEST
		m_gravitationPerturbed.reset(new GravitationPerturbed());
		m_graviHolder         = Function(m_gravitationPerturbed);
		m_graviJacobianHolder = Jacobian(m_gravitationJacobian);
		// END TEST

		//m_graviHolder         = Function(m_gravitation);
		//m_graviJacobianHolder = Jacobian(m_gravitationJacobian);
		
		//m_solver = solvers::gaussLegendre6<Float, 6>(1e-15, 50);
		m_solver = solvers::classic4<Float, 6>();
	}

	void PlanetSystem::update(ecs::Time t, ecs::Time dt)
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();

		auto* simulatorState = getSystemManager()->get<SimulatorState>();

		auto  planetEntity = simulatorState->getPlanet();
		auto& planetComp   = registry.get<Planet>(planetEntity);  

		// setup gravitation parameters
		//m_gravitation->setMu(planetComp.mu);
		//m_gravitationJacobian->setMu(planetComp.mu);

		// TEST
		m_gravitationPerturbed->setMu(planetComp.mu);
		m_gravitationPerturbed->setJ2(planetComp.J2);
		m_gravitationPerturbed->setR(planetComp.R);

		m_gravitationJacobian->setMu(planetComp.mu);
		// END TEST

		// calculating updates count
		auto updates = dt / m_dt;
		auto step = m_dt;
		if (updates > m_maxUpdates)
		{
			step = dt / m_maxUpdates;
			updates = m_maxUpdates;
		}
		auto stepSec = ecs::toSeconds<Float>(step).count();

		for (auto e : registry.view<comp::Orbit, comp::SimData, comp::PhysicsData>(entt::exclude<Planet>))
		{
			// orbit & simData & physics components of an updated entity
			auto [orbit, simData, physics] = registry.get<comp::Orbit, comp::SimData, comp::PhysicsData>(e);

			// current update time & current time delta that will be used
			auto tUpdate  = t;
			auto dtUpdate = dt;

			for (int i = 1; i < updates - 1; i++)
			{
				// TEST : setup
				orbit.setFromState({simData.getRadius(), simData.getVelocity()}, planetComp.mu);
				m_gravitationPerturbed->setAP(orbit.getOrbit().ap);
				m_gravitationPerturbed->setI(orbit.getOrbit().i);
				m_gravitationPerturbed->setTA(orbit.getOrbit().ta);
				m_gravitationPerturbed->setr(glm::length(simData.getRadius()));
				// END TEST

				auto tSec  = ecs::toSeconds<Float>(tUpdate).count();
				simData.state = m_solver.solve(m_graviHolder, m_graviJacobianHolder, tSec, simData.state, stepSec);

				tUpdate  += step;
				dtUpdate -= step;
			}
			// TEST : setup
			orbit.setFromState({simData.getRadius(), simData.getVelocity()}, planetComp.mu);
			m_gravitationPerturbed->setAP(orbit.getOrbit().ap);
			m_gravitationPerturbed->setI(orbit.getOrbit().i);
			m_gravitationPerturbed->setTA(orbit.getOrbit().ta);
			m_gravitationPerturbed->setr(glm::length(simData.getRadius()));
			// END TEST

			auto tSec  = ecs::toSeconds<Float>(tUpdate).count();
			auto dtSec = ecs::toSeconds<Float>(dtUpdate).count();
			simData.state = m_solver.solve(m_graviHolder, m_graviJacobianHolder, tSec, simData.state, dtSec);

			// update physics component
			physics.r = simData.getRadius();
			physics.v = simData.getVelocity();

			// final update for orbit
			orbit.setFromState({simData.getRadius(), simData.getVelocity()}, planetComp.mu);
		}
	}
	
	// TEST
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
	// END TEST
}