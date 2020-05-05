#include "SimulatorState.h"

#include <ECS/ecs_engine.h>
#include <ECS/System/SystemManager.h>
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
#include "../Components/Planet.h"
#include "../Components/Player.h"

// TEST
#include <SpaceUtils/ClohessyWiltshire.h>

#include "../Components/TestRendererTag.h"

#include "../Test.h"

#include "RendezvousControlSystem.h"
// END TEST

namespace
{	
	using ecs::entity::Entity;
	using ecs::entity::null;

	using math::Vec3f32;
	using math::Float;
	using math::F32;
	using math::F64;

	auto buildChaser(ecs::entity::EntityComponentRegistry& registry)
	{
		Entity satellite = registry.create();
		registry.assign<comp::Transform>      (satellite, test::CHA_TRANSLATE, test::CHA_ROTATION, test::CHA_SCALE);
		registry.assign<comp::PhysicsData>    (satellite, test::CHA_RAD, test::CHA_VEL, test::CHA_ROT_AXIS, test::CHA_ROT_ANGLE, test::CHA_MASS);
		registry.assign<comp::SimData>        (satellite, comp::fromRadVel(test::CHA_RAD, test::CHA_VEL));
		registry.assign<comp::Satellite>      (satellite, test::CHA_COLOR);
		registry.assign<comp::Orbit>          (satellite, comp::Orbit({test::CHA_RAD, test::CHA_VEL}, test::PLANET_MU));
		registry.assign<comp::TestRendererTag>(satellite);
		registry.assign<comp::Rendezvous>     (satellite, test::PROP_MASS, 0.0, test::PROP_ISP);

		return satellite;
	}

	auto buildTarget(ecs::entity::EntityComponentRegistry& registry)
	{
		Entity satellite = registry.create();
		registry.assign<comp::Transform>      (satellite, test::TAR_TRANSLATE, test::TAR_ROTATION, test::TAR_SCALE);
		registry.assign<comp::PhysicsData>    (satellite, test::TAR_RAD, test::TAR_VEL, test::TAR_ROT_AXIS, test::TAR_ROT_ANGLE, test::TAR_MASS);
		registry.assign<comp::SimData>        (satellite, comp::fromRadVel(test::TAR_RAD, test::TAR_VEL));
		registry.assign<comp::Satellite>      (satellite, test::TAR_COLOR);
		registry.assign<comp::Orbit>          (satellite, comp::Orbit({test::TAR_RAD, test::TAR_VEL}, test::PLANET_MU));
		registry.assign<comp::TestRendererTag>(satellite);
		return satellite;
	}

	auto buildPlayer(ecs::entity::EntityComponentRegistry& registry)
	{
		auto player = registry.create();
		registry.assign<comp::Player>         (player, player);
		registry.assign<comp::Transform>      (player, Vec3f32{1.5 * test::PLANET_R});
		registry.assign<comp::Camera3rdPerson>(player, Vec3f32{}, math::PI_3, 0.0, 0.0, 2.0, 2.0, 500.0);
		return player;
	}

	auto buildPlanet(ecs::entity::EntityComponentRegistry& registry)
	{
		Entity planet = registry.create();
		registry.assign<comp::Transform>      (planet, test::PLANET_RAD, test::PLANET_ROTATION, test::PLANET_SCALE);
		registry.assign<comp::PhysicsData>    (planet, test::PLANET_RAD, test::PLANET_VEL, test::PLANET_ROT_AXIS, test::PLANET_ROT_ANGLE, test::PLANET_MASS);
		registry.assign<comp::SimData>        (planet, comp::fromRadVel(test::PLANET_RAD, test::PLANET_VEL));
		registry.assign<comp::Planet>         (planet, test::PLANET_MU, test::PLANET_R, 1.0, test::PLANET_G0);
		registry.assign<comp::TestRendererTag>(planet);
		return planet;
	}
}

namespace sim
{
	SimulatorState::SimulatorState(ecs::sys::SystemManager* manager)
		:	base_t(manager)
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
		m_planet = buildPlanet(registry);
		m_player = buildPlayer(registry);

		m_target = buildTarget(registry);
		m_chaser = buildChaser(registry);
	}


	void SimulatorState::update(ecs::Time t, ecs::Time dt)
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* rendezvousControlSystem = sysManager->get<RendezvousControlSystem>();

		m_rendezvousStarted = rendezvousControlSystem->rendezvousStarted(m_chaser);

		// TODO : update player's camera
		// TODO : event system
		auto [player, camera] = registry.get<comp::Player, comp::Camera3rdPerson>(m_player);
		if (m_bindCamToNaturalAxes)
		{
			if (registry.valid(player.view) && registry.has<comp::PhysicsData>(player.view))
			{
				auto& physData = registry.get<comp::PhysicsData>(player.view);

				camera.setAxes(space_utils::naturalAxes(physData.r, physData.v));
			}
		}
		if (m_lastTimeBind && !m_bindCamToNaturalAxes)
		{
			camera.resetAxes();
		}
		//

		m_lastTimeBind = m_bindCamToNaturalAxes;
	}


	Entity SimulatorState::getPlanet() const
	{
		return m_planet;
	}

	Entity SimulatorState::getPlayer() const
	{
		return m_player;
	}

	Entity SimulatorState::getTarget() const
	{
		return m_target;
	}

	Entity SimulatorState::getChaser() const
	{
		return m_chaser;
	}


	bool SimulatorState::paused() const
	{
		return m_paused;
	}

	void SimulatorState::pause()
	{
		m_paused = true;
	}

	void SimulatorState::resume()
	{
		m_paused = false;
	}


	// TODO : remove
	// TEST
	void SimulatorState::startRendezvous(ecs::Time t, ecs::Time dt)
	{
		auto* sysManager = getSystemManager();
		auto* rendezvousControl = sysManager->get<RendezvousControlSystem>();

		m_rendezvousStarted = rendezvousControl->startRendezvous(m_target, m_chaser, t, dt);
	}

	void SimulatorState::abortRendezvous()
	{
		auto* sysManager = getSystemManager();
		auto* rendezvousControl = sysManager->get<RendezvousControlSystem>();
	
		rendezvousControl->abortRendezvous(m_chaser);

		m_rendezvousStarted = false;
	}

	bool SimulatorState::rendezvousStarted()
	{
		auto* sysManager = getSystemManager();
		auto* rendezvousControl = sysManager->get<RendezvousControlSystem>();

		return rendezvousControl->rendezvousStarted(m_chaser);
	}
	// END TEST

	bool SimulatorState::cameraBoundToNaturalAxes() const
	{
		return m_bindCamToNaturalAxes;
	}

	void SimulatorState::setCameraBindValue(bool bindToAxes)
	{
		m_bindCamToNaturalAxes = bindToAxes;
	}
}