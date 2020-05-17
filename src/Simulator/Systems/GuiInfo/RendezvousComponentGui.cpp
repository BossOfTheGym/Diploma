#include "RendezvousComponentGui.h"

#include "../SimulatorState.h"

namespace sim
{
	RendezvousComponentGui::RendezvousComponentGui(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{}

	void RendezvousComponentGui::render(Entity e)
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* simulatorState = sysManager->get<SimulatorState>();

		auto& rendComp = registry.get<comp::Rendezvous>(e);

		if (!simulatorState->paused())
		{
			ImGui::Text("Propellant mass: %lf", static_cast<double>(rendComp.propellantMass));
			ImGui::Text("Isp: %lf", static_cast<double>(rendComp.Isp));
		}
		else
		{
			char massStr[64];
			sprintf_s(massStr, "%.15lf", rendComp.propellantMass);
			ImGui::InputText("propellant", massStr, 64, ImGuiInputTextFlags_CharsDecimal);
			rendComp.propellantMass = atof(massStr);

			char ispStr[64];
			sprintf_s(ispStr, "%.15lf", rendComp.Isp);
			ImGui::InputText("Isp", ispStr, 64, ImGuiInputTextFlags_CharsDecimal);
			rendComp.Isp = atof(ispStr);
		}
		ImGui::Text("Propellant used: %lf", static_cast<double>(rendComp.propellantUsed));
		ImGui::Text("Target id: %u", entt::to_integral(e));
		ImGui::Text("Time remianing: %lf", ecs::toSeconds<double>(rendComp.duration).count());
	}

	const char* RendezvousComponentGui::name() const
	{
		return "Rendezvous component";
	}
}