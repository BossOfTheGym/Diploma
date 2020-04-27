#include "Simulator.h"

#include "Systems/ContextSystem.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/MeshSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/PlanetSystem.h"
#include "Systems/PlayerSystem.h"
#include "Systems/OrbitSystem.h"
#include "Systems/RendezvousControlSystem.h"
#include "Systems/TestRendererSystem.h"
#include "Systems/TimeSystem.h"
#include "Systems/ImGuiSystem.h"
#include "Systems/SimulatorState.h"
#include "Systems/GuiInfo/GuiInfo.h"

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
		OrbitSystem*             orbitSystem             = systemManager.get<OrbitSystem>();

		// main loop
		auto player = simulatorState->getPlayer();
		auto target = simulatorState->getPlanet();

		auto controller = playerSystem->getInputController();
		controller->setControlledEntity(player);

		getRegistry().get<comp::Player>(player).view = target;


		timeSystem->resetTime();
		contextSystem->showWindow();
		contextSystem->setSwapInterval(1);
		contextSystem->pushController(controller);
		while(!contextSystem->shouldClose())
		{
			contextSystem->pollEvents();
			timeSystem->tick();

			auto  t = timeSystem->getTime();
			auto dt = timeSystem->getDeltaTime();

			rendezvousControlSystem->update(t, dt);
			planetSystem->update(t, dt);
			physicsSystem->update(t, dt);

			orbitSystem->update(t, dt);
			playerSystem->update(t, dt);
			testRendererSystem->update(t, dt);
			imguiSystem->update(t, dt);

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

		// CORE : simulator state
		systemManager.add<SimulatorState>(&systemManager);

		// CORE : ImGuiSystem
		systemManager.add<ImGuiSystem>(&systemManager);
		// TODO : register components

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

		// OrbitSystem
		systemManager.add<OrbitSystem>(&systemManager);
	}
}