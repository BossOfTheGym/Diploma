#include "RendezvousControlGui.h"

#include "../SimulatorState.h"

namespace sim
{
	RendezvousSystemInfo::RendezvousSystemInfo(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{}

	void RendezvousSystemInfo::render()
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* rendezvousSystem = sysManager->get<RendezvousControlSystem>();
		auto* simulatorState   = sysManager->get<SimulatorState>();

		if (!simulatorState->paused())
		{
			ImGui::Text("Trajectory split: %d", rendezvousSystem->getSplit());
		}
		else
		{
			int split = rendezvousSystem->getSplit();
			ImGui::SliderInt("Trajectory split", &split, 1, 1000);
			rendezvousSystem->setSplit(split);
		}
	}

	const char* RendezvousSystemInfo::name() const
	{
		return "Rendezvous constrol system";
	}
}
