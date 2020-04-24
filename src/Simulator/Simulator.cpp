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
#include "Systems/ImGuiSystem.h"
#include "Systems/SimulatorState.h"
#include "Systems/GuiInfo/GuiInfo.h"

#include "EntityBuilders/ChaserFactory.h"
#include "EntityBuilders/PlanetFactory.h"
#include "EntityBuilders/SatelliteFactory.h"
#include "EntityBuilders/PlayerFactory.h"

#include "Components/Player.h"
#include "Components/Transform.h"
#include "Components/Camera.h"
#include "Components/Orbit.h"
#include "Components/Rendezvous.h"
#include "Components/Satellite.h"
#include "Components/Planet.h"
#include "Components/Transform.h"
#include "Components/PhysicsData.h"

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
		ContextSystem*           contextSystem           = systemManager.get<ContextSystem>();
		GraphicsSystem*          graphicsSystem          = systemManager.get<GraphicsSystem>();
		MeshSystem*              meshSystem              = systemManager.get<MeshSystem>();
		PhysicsSystem*           physicsSystem           = systemManager.get<PhysicsSystem>();
		PlanetSystem*            planetSystem            = systemManager.get<PlanetSystem>();
		TestRendererSystem*      testRendererSystem      = systemManager.get<TestRendererSystem>();
		TimeSystem*              timeSystem              = systemManager.get<TimeSystem>();		
		PlayerSystem*            playerSystem            = systemManager.get<PlayerSystem>();
		RendezvousControlSystem* rendezvousControlSystem = systemManager.get<RendezvousControlSystem>();
		ImGuiSystem*             imguiSystem             = systemManager.get<ImGuiSystem>();
		SimulatorState*          simulatorState          = systemManager.get<SimulatorState>();

		// builders
		

		// main loop
		// init
		// TODO
		auto player = simulatorState->getPlayer();
		auto sat1   = simulatorState->getSat1();

		auto controller = playerSystem->getInputController();
		controller->setControlledEntity(player);

		getRegistry().get<comp::Player>(player).view = sat1;


		contextSystem->showWindow();
		contextSystem->setSwapInterval(1);
		contextSystem->pushController(controller);
		while(!contextSystem->shouldClose())
		{
			contextSystem->pollEvents();
			timeSystem->tick();

			auto  t = timeSystem->getTime();
			auto dt = timeSystem->getDeltaTime();

			planetSystem->update(t, dt);
			physicsSystem->update(t, dt);
			playerSystem->update(t, dt);
			testRendererSystem->update(t, dt);

			// TEST
			imguiSystem->update(t, dt);
			// END TEST

			contextSystem->swapBuffers();
		}

		// deinit
		m_registry.clear();
	}


	void Simulator::init()
	{
		// TODO : TODO : TODO
	}

	void Simulator::deinit()
	{
		// TODO : TODO : TODO
	}

	void Simulator::loadEntityBuilders()
	{
		// TODO : TODO : TODO
	}

	void Simulator::loadSystems()
	{
		auto& systemManager = getSystemManager();

		// CORE : context
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

		// CORE : graphics
		systemManager.add<GraphicsSystem>(&systemManager);

		// CORE : mesh system
		systemManager.add<MeshSystem>(&systemManager);

		// CORE : Time system
		using Tick = TimeSystem::Tick;
		Tick warp = 1;
		Tick minWarp = 1;
		Tick maxWarp = 1;
		systemManager.add<TimeSystem>(&systemManager, warp, minWarp, maxWarp);

		// TODO : initialization order
		auto& entityBuilderManager = getEntityBuilderManager();

		// ChaserFactory
		entityBuilderManager.add<ChaserFactory>(&entityBuilderManager);

		// SatelliteFactory
		entityBuilderManager.add<SatelliteFactory>(&entityBuilderManager);

		// PlanetFactory
		entityBuilderManager.add<PlanetFactory>(&entityBuilderManager);

		// PlayerFactory
		entityBuilderManager.add<PlayerFactory>(&entityBuilderManager);
		// TODOTODOTODOTODOTODO

		// CORE : simulator state
		systemManager.add<SimulatorState>(&systemManager);


		// PlanetSystem
		systemManager.add<PlanetSystem>(&systemManager);

		// Rendezvous control system
		systemManager.add<RendezvousControlSystem>(&systemManager);

		// TestRendererSystem
		systemManager.add<TestRendererSystem>(&systemManager);

		// Physics system
		systemManager.add<PhysicsSystem>(&systemManager);

		// Player system
		systemManager.add<PlayerSystem>(&systemManager);

		// ImGuiSystem
		systemManager.add<ImGuiSystem>(&systemManager);
		auto* imguiSys = systemManager.get<ImGuiSystem>();
		//imguiSys->registerComponent<comp::
	}
}