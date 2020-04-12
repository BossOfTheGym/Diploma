#include "Simulator.h"

#include "Systems/ContextSystem.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/MeshSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlanetSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/RendezvousControlSystem.h"
#include "Systems/TestRendererSystem.h"
#include "Systems/TimeSystem.h"

#include "EntityBuilders/ChaserFactory.h"
#include "EntityBuilders/PlanetFactory.h"
#include "EntityBuilders/SatelliteFactory.h"
#include "EntityBuilders/PlayerFactory.h"

#include "Components/Player.h"
#include "Components/Transform.h"
#include "Components/Camera.h"

namespace sim
{
	Simulator::Simulator()
	{
		loadSystems(); // loaded first
		loadEntityBuilders();
	}

	void Simulator::mainLoop()
	{
		// systems
		auto& systemManager = getSystemManager();

		ContextSystem* contextSystem = systemManager.get<ContextSystem>();

		GraphicsSystem* graphicsSystem = systemManager.get<GraphicsSystem>();

		MeshSystem* meshSystem = systemManager.get<MeshSystem>();

		PhysicsSystem* physicsSystem = systemManager.get<PhysicsSystem>();

		PlanetSystem* planetSystem = systemManager.get<PlanetSystem>();

		TestRendererSystem* testRendererSystem = systemManager.get<TestRendererSystem>();

		TimeSystem* timeSystem = systemManager.get<TimeSystem>();		

		PlayerSystem* playerSystem = systemManager.get<PlayerSystem>();


		// builders
		auto& entityBuilderManager = getEntityBuilderManager();

		SatelliteFactory* satelliteFactory = entityBuilderManager.get<SatelliteFactory>();

		PlanetFactory* planetFactory = entityBuilderManager.get<PlanetFactory>();

		PlayerFactory* playerFactory = entityBuilderManager.get<PlayerFactory>();

		// main loop
		// init
		m_planet = planetFactory->buildEntity();
		m_player = playerFactory->buildEntity();

		m_sat1 = satelliteFactory->buildEntity();

		auto controller = playerSystem->getInputController();
		controller->setControlledEntity(m_player);

		getRegistry().get<comp::Player>(m_player).view = m_sat1;


		contextSystem->showWindow();
		contextSystem->setSwapInterval(1);
		contextSystem->pushController(controller);
		while(!contextSystem->shouldClose())
		{
			contextSystem->pollEvents();
			timeSystem->tick();

			auto  t = timeSystem->getTime() / static_cast<Float>(TimeSystem::PERIOD);
			auto dt = timeSystem->getDeltaTime() / static_cast<Float>(TimeSystem::PERIOD);

			planetSystem->update(t, dt);
			physicsSystem->update(t, dt);
			playerSystem->update(t, dt);
			testRendererSystem->update(t, dt);

			contextSystem->swapBuffers();
		}

		// deinit
		m_registry.clear();

		m_planet = null;
		m_player = null;
		m_sat1 = null;
		m_sat2 = null;
	}


	void Simulator::init()
	{
		// TODO
	}

	void Simulator::deinit()
	{
		// TODO
	}

	void Simulator::loadEntityBuilders()
	{
		auto& entityBuilderManager = getEntityBuilderManager();

		// ChaserFactory
		entityBuilderManager.add<ChaserFactory>(&entityBuilderManager);

		// SatelliteFactory
		entityBuilderManager.add<SatelliteFactory>(&entityBuilderManager);

		// PlanetFactory
		entityBuilderManager.add<PlanetFactory>(&entityBuilderManager);

		// PlayerFactory
		entityBuilderManager.add<PlayerFactory>(&entityBuilderManager);
	}

	void Simulator::loadSystems()
	{
		auto& systemManager = getSystemManager();

		// context
		CreationInfo info{
			  1280
			, 720
			, "Simulator"
			, Hints{
				  {GLFW_DOUBLEBUFFER, GLFW_TRUE}
				, {GLFW_CENTER_CURSOR, GLFW_TRUE}
				, {GLFW_DEPTH_BITS  , 24}
				, {GLFW_STENCIL_BITS, 8}
				, {GLFW_CONTEXT_VERSION_MAJOR, 4}
				, {GLFW_CONTEXT_VERSION_MINOR, 5}
				, {GLFW_VISIBLE  , GLFW_FALSE}
				, {GLFW_RESIZABLE, GLFW_FALSE}
			}
		};
		Float fovy = glm::radians(45.0);
		Float near = 1.0;
		Float far  = 1000.0;
		systemManager.add<ContextSystem>(&systemManager, info, true, fovy, near, far);

		// graphics
		systemManager.add<GraphicsSystem>(&systemManager);

		// mesh system
		systemManager.add<MeshSystem>(&systemManager);

		// PlanetSystem
		systemManager.add<PlanetSystem>(&systemManager);

		// Player System
		//systemManager.add<PlayerSystem>(&systemManager);

		// Rendezvous control system
		//systemManager.add<RendezvousControlSystem>(&systemManager);

		// TestRendererSystem
		systemManager.add<TestRendererSystem>(&systemManager);

		// Time system
		using Tick = TimeSystem::Tick;
		Tick warp = 1;
		Tick minWarp = 1;
		Tick maxWarp = 1;
		systemManager.add<TimeSystem>(&systemManager, warp, minWarp, maxWarp);

		// Physics system
		systemManager.add<PhysicsSystem>(&systemManager);

		// Player system
		systemManager.add<PlayerSystem>(&systemManager);
	}


	Entity Simulator::getPlanet() const
	{
		return m_planet;
	}

	Entity Simulator::getPlayer() const
	{
		return m_player;
	}

	Entity Simulator::getSat1() const
	{
		return m_sat1;
	}

	Entity Simulator::getSat2() const
	{
		return m_sat2;
	}
}