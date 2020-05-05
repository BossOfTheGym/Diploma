#include "PhysicsComponentGui.h"

namespace sim
{
	PhysicsComponentGui::PhysicsComponentGui(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{}


	void PhysicsComponentGui::render(Entity e)
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto& physics = registry.get<comp::PhysicsData>(e);

		ImGui::Text("r x: %lf y: %lf z: %lf", (double)physics.r.x, (double)physics.r.y, (double)physics.r.z);
		ImGui::Text("v x: %lf y: %lf z: %lf", (double)physics.v.x, (double)physics.v.y, (double)physics.v.z);
	}

	const char* PhysicsComponentGui::name() const
	{
		return "Physics";
	}
}