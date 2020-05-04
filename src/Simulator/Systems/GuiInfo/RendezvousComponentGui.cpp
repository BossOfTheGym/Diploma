#include "RendezvousComponentGui.h"


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

		auto& rendComp = registry.get<comp::Rendezvous>(e);

		ImGui::Text("Propellant mass: %lf", static_cast<double>(rendComp.propellantMass));
		ImGui::Text("Propellant used: %lf", static_cast<double>(rendComp.propellantUsed));
		ImGui::Text("Isp: %lf", static_cast<double>(rendComp.Isp));
		ImGui::Text("Target id: %u", entt::to_integral(e));
		ImGui::Text("Time remianing: %lf", ecs::toSeconds<double>(rendComp.duration).count());
	}

	const char* RendezvousComponentGui::name() const
	{
		return "Rendezvous component";
	}
}