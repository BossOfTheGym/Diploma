#include "SatelliteFactory.h"

#include <ECS/Entity/EntityBuilderManager.h>

#include "../Components/Camera.h"
#include "../Components/Transform.h"
#include "../Components/PhysicsData.h"
#include "../Components/SimData.h"
#include "../Components/Satellite.h"
#include "../Components/Orbit.h"
#include "../Components/MeshComponent.h"
#include "../Components/TestRendererTag.h"

#include "../Simulator.h"
#include "../Systems/ContextSystem.h"

#include "../Test.h"


// TODO
namespace sim
{
	SatelliteFactory::SatelliteFactory(ecs::entity::EntityBuilderManager* manager)
		: base_t(manager)
	{
		m_simulator = static_cast<Simulator*>(manager->getEngineInstance());
	}

	Entity SatelliteFactory::buildEntity()
	{
		auto& registry      = m_simulator->getRegistry();
		auto& systemManager = m_simulator->getSystemManager();

		auto* contextSystem = systemManager.get<ContextSystem>();
		if (!contextSystem)
		{
			return null;
		}

		Entity satellite = registry.create();
		registry.assign<comp::Camera3rdPerson>(satellite, test::SAT_TRANSLATE, math::PI_4, 0.0, 0.0, 1.0, 0.5, 10.0);
		registry.assign<comp::Transform>      (satellite, test::SAT_TRANSLATE, test::SAT_ROTATION, test::SAT_SCALE);
		registry.assign<comp::PhysicsData>    (satellite, test::SAT_RAD, test::SAT_VEL, test::SAT_ROT_AXIS, test::SAT_ROT_ANGLE, test::SAT_MASS);
		registry.assign<comp::SimData>        (satellite, comp::fromRadVel(test::SAT_RAD, test::SAT_VEL));
		registry.assign<comp::Satellite>      (satellite, test::SAT_COLOR);
		//registry.assign<comp::Orbit>          (satellite); // TODO
		//registry.assign<comp::MeshComponent>  (satellite); // TODO
		registry.assign<comp::TestRendererTag>(satellite);
		return satellite;
	}
}