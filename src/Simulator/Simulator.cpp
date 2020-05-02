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
#include "Systems/GuiInfo/OrbitComponentGui.h"
#include "Systems/GuiInfo/RendezvousControlGui.h"
#include "Systems/GuiInfo/SimulatorStateGui.h"
#include "Systems/GuiInfo/TimeSystemGui.h"
#include "Systems/GuiInfo/PlanetComponentGui.h"

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
		auto planet = simulatorState->getPlanet();
		auto target = simulatorState->getTarget();
		auto chaser = simulatorState->getChaser();

		auto controller = playerSystem->getInputController();
		controller->setControlledEntity(player);
		contextSystem->pushController(controller);

		getRegistry().get<comp::Player>(player).view = target;

		imguiSystem->registerEntity(planet);
		imguiSystem->registerEntity(target);
		imguiSystem->registerEntity(chaser);
		imguiSystem->registerEntity(player);

		timeSystem->resetTime();
		contextSystem->showWindow();
		contextSystem->setSwapInterval(1);

		Time t{};
		Time dt{};
		while(!contextSystem->shouldClose())
		{
			contextSystem->pollEvents();
			timeSystem->tick();

			if (!simulatorState->paused())
			{
				t   = timeSystem->getTime();
				dt += timeSystem->getDeltaTime();

				Time tf = t + dt; // t-ime f-inal
				while(timeSystem->hasTimeEvents() && timeSystem->peekTimeEvent() < tf)
				{
					Time ts = timeSystem->peekTimeEvent(); // t-ime s-afe
					while(timeSystem->hasTimeEvents() && timeSystem->peekTimeEvent() == ts)
					{
						timeSystem->popTimeEvent();
					}
					
					Time dts = ts - t; // dt s-afe

					// TODO : move to seperate method
					rendezvousControlSystem->update(t, dts);
					planetSystem->update(t, dts);
					physicsSystem->update(t, dts);
					orbitSystem->update(t, dts);

					t  += dts; // progress time
					dt -= dts; // consume time
					///////////
				}
				// TODO : move to seperate method
				rendezvousControlSystem->update(t, dt);
				planetSystem->update(t, dt);
				physicsSystem->update(t, dt);
				orbitSystem->update(t, dt);

				t  += dt; // progress time
				dt -= dt; // consume time
				///////////
			}
			
			// TODO : solve this problem with updating
			// TODO : renderers should render object in-simulator state so should be updated using sim time
			// TODO : add ability to time system to stop the time: real time updates always, warped(simulator, rename)
			//		is updated only if simulation not stopped
			Time tr  = timeSystem->getRealTime(); // t-ime r-eal
			Time dtr = timeSystem->getRealTimeDelta();

			playerSystem->update(tr, dtr);
			testRendererSystem->update(tr, dtr);
			imguiSystem->update(tr, dtr);
			simulatorState->update(t, dt);

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
			  1800
			, 900
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
		Float near = 0.01;
		Float far  = 6000.0;
		systemManager.add<ContextSystem>(info, true, fovy, near, far);

		// CORE : graphics
		systemManager.add<GraphicsSystem>();

		// CORE : mesh system
		systemManager.add<MeshSystem>();

		// CORE : Time system
		using Tick = TimeSystem::Tick;
		Tick warp = 1;
		Tick minWarp = 1;
		Tick maxWarp = 1000;
		systemManager.add<TimeSystem>(warp, minWarp, maxWarp);

		// CORE : simulator state
		systemManager.add<SimulatorState>();

		// CORE : ImGuiSystem
		systemManager.add<ImGuiSystem>();

		auto* imGuiSystem = systemManager.get<ImGuiSystem>();
		imGuiSystem->registerSystem<RendezvousSystemInfo>();
		imGuiSystem->registerSystem<SimulatorStateGui>();
		imGuiSystem->registerSystem<TimeSystemInfo>();
		imGuiSystem->registerComponent<OrbitComponentGui>();
		imGuiSystem->registerComponent<PlanetComponentGui>();

		// PlanetSystem
		systemManager.add<PlanetSystem>();

		// Rendezvous control system
		systemManager.add<RendezvousControlSystem>();

		// TestRendererSystem
		systemManager.add<TestRendererSystem>();

		// Physics system
		systemManager.add<PhysicsSystem>();

		// Player system
		systemManager.add<PlayerSystem>();

		// OrbitSystem
		systemManager.add<OrbitSystem>();
	}
}