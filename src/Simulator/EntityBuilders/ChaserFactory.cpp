#include "ChaserFactory.h"

#include <ECS/Entity/EntityBuilderManager.h>

#include "../Components/Camera.h"
#include "../Components/Transform.h"
#include "../Components/PhysicsData.h"
#include "../Components/SimData.h"
#include "../Components/Satellite.h"
#include "../Components/Orbit.h"
#include "../Components/MeshComponent.h"
#include "../Components/TestRendererTag.h"
#include "../Components/Rendezvous.h"
#include "../Components/Orbit.h"

#include "../Simulator.h"
#include "../Test.h"

// TODO
namespace sim
{
	ChaserFactory::ChaserFactory(ecs::entity::EntityBuilderManager* manager)
		: base_t(manager)
	{
		// TODO : assert
		m_simulator = static_cast<Simulator*>(manager->getEngineInstance());
	}

	Entity ChaserFactory::buildEntity()
	{
		auto& registry = m_simulator->getRegistry();

		Entity satellite = registry.create();
		registry.assign<comp::Transform>      (satellite);
		registry.assign<comp::PhysicsData>    (satellite);
		registry.assign<comp::SimData>        (satellite);
		registry.assign<comp::Satellite>      (satellite);
		registry.assign<comp::Orbit>          (satellite);
		registry.assign<comp::MeshComponent>  (satellite);
		registry.assign<comp::Rendezvous>     (satellite);
		registry.assign<comp::Orbit>          (satellite);
		registry.assign<comp::TestRendererTag>(satellite);
		return satellite;
	}
}