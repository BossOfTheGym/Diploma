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

#include "TimeSystem.h"
#include "RendezvousControlSystem.h"
#include "PlanetSystem.h"

// TEST
#include <SpaceUtils/ClohessyWiltshire.h>

#include "../Components/TestRendererTag.h"

#include "../Test.h"

// END TEST

#include <fstream>
#include <iostream>
#include <ctime>

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
		registry.assign<comp::Planet>         (planet, test::PLANET_MU, test::PLANET_R, 1.0, test::PLANET_G0, test::PLANET_J2);
		registry.assign<comp::TestRendererTag>(planet);
		return planet;
	}

	using math::Vec3;

	bool writeLogData(
		  std::string fileName
		, std::vector<Vec3> chaserRad
		, std::vector<Vec3> chaserVel
		, std::vector<Vec3> targetRad
		, std::vector<Vec3> targetVel
		, std::vector<Vec3> dvImpuls
		, std::vector<Vec3> chaserRadImp
		, std::vector<Vec3> chaserVelImp
		, std::vector<Vec3> targetRadImp
		, std::vector<Vec3> targetVelImp
		, Float mass)
	{
		std::ofstream output(fileName);
		if (!output.is_open())
		{
			return false;
		}

		output << "[[chaser]]" << std::endl;
		int n = chaserRad.size();
		for (int i = 0; i < n; i++)
		{
			auto& rad = chaserRad[i];
			auto& vel = chaserVel[i];

			output << rad.x << " " << rad.y << " " << rad.z << " " 
				<< vel.x << " " << vel.y << " " << vel.z << std::endl;
		}

		output << "[[target]]" << std::endl;
		n = chaserRad.size();
		for (int i = 0; i < n; i++)
		{
			auto& rad = targetRad[i];
			auto& vel = targetVel[i];

			output << rad.x << " " << rad.y << " " << rad.z << " " 
				<< vel.x << " " << vel.y << " " << vel.z << std::endl;
		}
		
		output << "[[impulses]]" << std::endl;
		n = dvImpuls.size();
		for(int i = 0; i < n; i++)
		{
			auto& dv = dvImpuls[i];
			auto& crad = chaserRadImp[i];
			auto& cvel = chaserVelImp[i];
			auto& trad = targetRadImp[i];
			auto& tvel = targetVelImp[i];

			output << dv.x << " " << dv.y << " " << dv.z << " "
				<< crad.x << " " << crad.y << " " << crad.z << " "
				<< cvel.x << " " << cvel.y << " " << cvel.z << " "
				<< trad.x << " " << trad.y << " " << trad.z << " "
				<< tvel.x << " " << tvel.y << " " << tvel.z << std::endl;
		}

		output << "[[mass]]" << std::endl;
		output << mass << std::endl;

		return true;
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

		m_baseName = "simLog/log";
	}


	void SimulatorState::update(ecs::Time t, ecs::Time dt)
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* rendezvousControlSystem = sysManager->get<RendezvousControlSystem>();

		m_rendezvousStarted = rendezvousControlSystem->rendezvousStarted(m_chaser);
		if (!m_lastRendezvousStarted && m_rendezvousStarted)
		{
			m_logUpdates = 0;
		}
		if (m_rendezvousStarted)
		{
			if (registry.valid(m_target) && registry.has<comp::SimData>(m_target)
				&& registry.valid(m_chaser) && registry.has<comp::SimData>(m_chaser)
				&& !paused())
			{
				auto& chaserSimData = registry.get<comp::SimData>(m_chaser);
				auto& targetSimData = registry.get<comp::SimData>(m_target);

				m_chaserRadLog.push_back(chaserSimData.getRadius());
				m_chaserVelLog.push_back(chaserSimData.getVelocity());
				m_targetRadLog.push_back(targetSimData.getRadius());
				m_targetVelLog.push_back(targetSimData.getVelocity());
			}
			m_logUpdates++;
		}
		if (m_lastRendezvousStarted && !m_rendezvousStarted)
		{
			if (m_logWritten.valid())
			{
				if (!m_logWritten.get())
				{
					std::cout << "Simulator state: failed to write log file" << std::endl; 
				}
			}

			// TODO : safe checks
			auto& rendComp = registry.get<comp::Rendezvous>(m_chaser);
			m_logWritten = std::async(
				  std::launch::async
				, &writeLogData
				, m_baseName + "_" + std::to_string(m_fileNum) + ".log"
				, std::move(m_chaserRadLog)
				, std::move(m_chaserVelLog)
				, std::move(m_targetRadLog)
				, std::move(m_targetVelLog)
				, std::move(m_dvImpulses)
				, std::move(m_chaserRadImpLog)
				, std::move(m_chaserVelImpLog)
				, std::move(m_targetRadImpLog)
				, std::move(m_targetVelImpLog)
				, rendComp.propellantUsed
			);
			m_fileNum++;
		}
		m_lastRendezvousStarted = m_rendezvousStarted;

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
		auto* sysManager = getSystemManager();
		auto* timeSystem = sysManager->get<TimeSystem>();

		timeSystem->stop();
		m_paused = true;
	}

	void SimulatorState::resume()
	{
		auto* sysManager = getSystemManager();
		auto* timeSystem = sysManager->get<TimeSystem>();

		timeSystem->resume();
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

	//void SimulatorState::startLambertTransfer(const Vec3& dest, ecs::Time dt)
	//{
	//	auto* sysManager = getSystemManager();
	//	auto* rendezvousControl = sysManager->get<RendezvousControlSystem>();
	//
	//	m_rendezvousStarted = rendezvousControl->startLambertTransfer(m_chaser, dest, dt);
	//}

	void SimulatorState::resetTestState(ecs::Time t, ecs::Time dt, int impulses)
	{
		auto* sysManager = getSystemManager();
		
		auto* rendSystem = sysManager->get<RendezvousControlSystem>();
		auto* timeSystem = sysManager->get<TimeSystem>();

		rendSystem->setSplit(impulses);

		resetTestState();
		startRendezvous(timeSystem->getTime(), dt);
	}

	void SimulatorState::resetTestState()
	{
		auto* sysManager = getSystemManager();
		auto* ecsEngine = sysManager->getECSEngine();
		auto& registry = ecsEngine->getRegistry();

		auto* planetSystem = sysManager->get<PlanetSystem>();
		auto* timeSystem   = sysManager->get<TimeSystem>();

		if (!registry.valid(m_chaser) || !registry.has<comp::SimData>(m_chaser) || !registry.has<comp::Orbit>(m_chaser) || !registry.has<comp::Rendezvous>(m_chaser)
			|| !registry.valid(m_target) || !registry.has<comp::SimData>(m_target) || !registry.has<comp::Orbit>(m_target)
			|| !registry.valid(m_planet) || !registry.has<comp::Planet>(m_planet))
		{
			return;
		}

		auto [chaserSimData, chaserOrbit, chaserRend] = registry.get<comp::SimData, comp::Orbit, comp::Rendezvous>(m_chaser);
		auto [targetSimData, targetOrbit]             = registry.get<comp::SimData, comp::Orbit>(m_target);

		auto& planet = registry.get<comp::Planet>(m_planet);

		chaserOrbit.setFromParameters(51000.0, glm::radians(50.0), glm::radians(0.0), 0.001, glm::radians(0.0), glm::radians(10.0), planet.mu);
		chaserSimData.setRadius(chaserOrbit.getState().r);
		chaserSimData.setVelocity(chaserOrbit.getState().v);

		targetOrbit.setFromParameters(51000.0, glm::radians(50.0), glm::radians(0.0), 0.001, glm::radians(0.0), glm::radians(10.5), planet.mu);
		targetSimData.setRadius(targetOrbit.getState().r);
		targetSimData.setVelocity(targetOrbit.getState().v);

		chaserRend.propellantMass = 500.0;
		chaserRend.propellantUsed = 0.0;
		chaserRend.Isp = 400.0;

		planetSystem->updateEntity(m_chaser);
		planetSystem->updateEntity(m_target);

		timeSystem->resetTime();
	}

	void SimulatorState::logDvImpuls(const Vec3& dv)
	{
		auto* sysManager = getSystemManager();
		auto* ecsEngine = sysManager->getECSEngine();
		auto& registry = ecsEngine->getRegistry();

		if (registry.valid(m_chaser) && registry.has<comp::SimData>(m_chaser)
			&& registry.valid(m_target) && registry.has<comp::SimData>(m_target))
		{
			auto& chaserSimData = registry.get<comp::SimData>(m_chaser);
			auto& targetSimData = registry.get<comp::SimData>(m_target);

			m_dvImpulses.push_back(dv);
			m_chaserRadImpLog.push_back(chaserSimData.getRadius());
			m_chaserVelImpLog.push_back(chaserSimData.getVelocity());
			m_targetRadImpLog.push_back(targetSimData.getRadius());
			m_targetVelImpLog.push_back(targetSimData.getVelocity());
		}
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