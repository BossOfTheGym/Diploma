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
		auto& registry = m_simulator->getRegistry();

		Entity satellite = registry.create();
		registry.assign<comp::Camera3rdPerson>(satellite);
		registry.assign<comp::Transform>      (satellite);
		registry.assign<comp::PhysicsData>    (satellite);
		registry.assign<comp::SimData>        (satellite);
		registry.assign<comp::Satellite>      (satellite);
		registry.assign<comp::Orbit>          (satellite);
		registry.assign<comp::MeshComponent>  (satellite);
		registry.assign<comp::TestRendererTag>(satellite);
		return satellite;
	}
}