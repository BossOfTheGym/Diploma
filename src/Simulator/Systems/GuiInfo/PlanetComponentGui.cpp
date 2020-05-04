#include "PlanetComponentGui.h"

#include "../SimulatorState.h"

namespace sim
{
	PlanetComponentGui::PlanetComponentGui(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
	}

	void PlanetComponentGui::render(Entity e)
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* simulatorState = sysManager->get<SimulatorState>();


		auto& planet = registry.get<comp::Planet>(e);

		if (simulatorState->paused())
		{
			char muStr[64];
			sprintf_s(muStr, "%.15f", planet.mu);

			ImGui::InputText("mu", muStr, 64, ImGuiInputTextFlags_CharsDecimal);

			planet.mu = atof(muStr);	
		}
		else
		{
			ImGui::Text( "mu: %.6f", planet.mu);
		}
		ImGui::Text("R : %.6f", planet.R);
		ImGui::Text("g0: %.6f", planet.g0);
	}

	const char* PlanetComponentGui::name() const
	{
		return "Planet component";
	}
}