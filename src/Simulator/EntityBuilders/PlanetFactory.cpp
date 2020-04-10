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

#include "../Test.h"

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
		registry.assign<comp::Camera3rdPerson>(planet, test::PLANET_RAD, math::PI_4, 0.0, 0.0, test::PLANET_R, test::PLANET_R * 1.5, test::PLANET_R * 10.0);
		registry.assign<comp::Transform>      (planet, test::PLANET_RAD, test::PLANET_ROTATTION, test::PLANET_SCALE);
		registry.assign<comp::PhysicsData>    (planet, test::PLANET_RAD, test::PLANET_VEL, test::PLANET_ROT_AXIS, test::PLANET_ROT_ANGLE, test::PLANET_MASS);
		registry.assign<comp::SimData>        (planet, comp::fromRadVel(test::PLANET_RAD, test::PLANET_VEL));
		//registry.assign<comp::MeshComponent>  (planet); // TODO
		registry.assign<comp::Planet>         (planet, test::PLANET_MU, test::PLANET_R, 1.0);
		registry.assign<comp::TestRendererTag>(planet);
		return planet;
	}
}