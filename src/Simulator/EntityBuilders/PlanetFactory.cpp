#include "PlanetFactory.h"

#include <ECS/Entity/EntityBuilderManager.h>

#include "../Components/Camera.h"
#include "../Components/Transform.h"
#include "../Components/PhysicsData.h"
#include "../Components/SimData.h"
#include "../Components/MeshComponent.h"
#include "../Components/Planet.h"

// TEST
#include "../Components/TestRendererTag.h"

#include "../Simulator.h"

// TODO
namespace sim
{
	PlanetFactory::PlanetFactory(ecs::entity::EntityBuilderManager* manager)
		: base_t(manager)
	{
		// TODO : assert
		m_simulator = static_cast<Simulator*>(manager->getEngineInstance());
	}

	Entity PlanetFactory::buildEntity()
	{
		auto& registry = m_simulator->getRegistry();

		Entity planet = registry.create();
		registry.assign<comp::Camera3rdPerson>(planet);
		registry.assign<comp::Transform>      (planet);
		registry.assign<comp::PhysicsData>    (planet);
		registry.assign<comp::SimData>        (planet);
		registry.assign<comp::MeshComponent>  (planet);
		registry.assign<comp::Planet>         (planet);
		registry.assign<comp::TestRendererTag>(planet);
		return planet;
	}
}