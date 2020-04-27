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

		char muStr[64];
		sprintf_s(muStr, "%.15f", planet.mu);

		if (simulatorState->paused())
		{
			ImGui::InputText("planet mu", muStr, 64, ImGuiInputTextFlags_CharsDecimal);
			// TODO : planet radius
			// TODO : planet angular velocity

			planet.mu = atof(muStr);	
		}
		else
		{
			ImGui::Text(muStr);
		}
	}

	const char* PlanetComponentGui::name() const
	{
		return "Planet component";
	}
}