#include "PlayerComponentGui.h"

namespace sim
{
	PlayerComponentGui::PlayerComponentGui(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{}

	void PlayerComponentGui::render(Entity e)
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto& player = registry.get<comp::Player>(e);

		ImGui::Text("View: %u", entt::to_integral(player.view));
	}

	const char* PlayerComponentGui::name() const
	{
		return "Player component";
	}
}