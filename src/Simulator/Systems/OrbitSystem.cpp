#include "OrbitSystem.h"

#include <ECS/System/SystemManager.h>
#include <ECS/Entity/Entity.h>
#include <ECS/ecs_engine.h>

#include "../Components/Planet.h"
#include "../Components/SimData.h"
#include "../Components/Orbit.h"

#include "SimulatorState.h"

namespace sim
{
	OrbitSystem::OrbitSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{}

	void OrbitSystem::update(ecs::Time t, ecs::Time dt)
	{
		// TODO : remove this system
		// TODO : unite with planet system

		//auto* sysManager = getSystemManager();
		//auto* ecsEngine  = sysManager->getECSEngine();
		//auto& registry   = ecsEngine->getRegistry();
		//
		//auto* simulatorState = sysManager->get<SimulatorState>();
		//
		//auto planetEntity = simulatorState->getPlanet();
		//if (!registry.valid(planetEntity) || !registry.has<comp::Planet>(planetEntity))
		//{
		//	// TODO : notify about error
		//	return;
		//}
		//auto& planet = registry.get<comp::Planet>(planetEntity);
		//
		//for (auto e : registry.view<comp::Orbit, comp::SimData>())
		//{
		//	auto [orbit, simData] = registry.get<comp::Orbit, comp::SimData>(e);
		//	orbit.setFromState({simData.getRadius(), simData.getVelocity()}, planet.mu);
		//}
	}
}