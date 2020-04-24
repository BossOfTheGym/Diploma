#include "SimulatorState.h"

#include <ECS/ecs_engine.h>
#include <ECS/System/SystemManager.h>
#include <ECS/Entity/EntityBuilderManager.h>

#include "../EntityBuilders/ChaserFactory.h"
#include "../EntityBuilders/PlanetFactory.h"
#include "../EntityBuilders/SatelliteFactory.h"
#include "../EntityBuilders/PlayerFactory.h"

namespace sim
{
	SimulatorState::SimulatorState(ecs::sys::SystemManager* manager)
		:	base_t(manager)
	{
		auto& entityBuilderManager = manager->getECSEngine()->getEntityBuilderManager();
		SatelliteFactory* satelliteFactory = entityBuilderManager.get<SatelliteFactory>();
		PlanetFactory*    planetFactory    = entityBuilderManager.get<PlanetFactory>();
		PlayerFactory*    playerFactory    = entityBuilderManager.get<PlayerFactory>();

		m_planet = planetFactory->buildEntity();
		m_player = playerFactory->buildEntity();

		m_sat1 = satelliteFactory->buildEntity();
	}

	Entity SimulatorState::getPlanet() const
	{
		return m_planet;
	}

	Entity SimulatorState::getPlayer() const
	{
		return m_player;
	}

	Entity SimulatorState::getSat1() const
	{
		return m_sat1;
	}

	Entity SimulatorState::getSat2() const
	{
		return m_sat2;
	}
}