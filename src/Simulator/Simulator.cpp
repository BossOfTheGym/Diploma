
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

		// builders
		auto& entityBuilderManager = getEntityBuilderManager();

		SatelliteFactory* satelliteFactory = entityBuilderManager.get<SatelliteFactory>();

		PlanetFactory* planetFactory = entityBuilderManager.get<PlanetFactory>();


		// main loop
		while(!contextSystem->shouldClose())
		{
			contextSystem->pollEvents();
			timeSystem->tick();


		}
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
	}
}