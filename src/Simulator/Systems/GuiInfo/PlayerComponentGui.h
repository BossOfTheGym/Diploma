#pragma once

#include "GuiInfo.h"

#include "../../Components/Player.h"

namespace sim
{
	class PlayerComponentGui : public ComponentInfo<comp::Player, PlayerComponentGui>
	{
	public:
		using this_t = PlayerComponentGui;
		using base_t = ComponentInfo<comp::Player, this_t>;

	public:
		PlayerComponentGui(ecs::sys::SystemManager* manager);

	public:
		virtual void render(Entity e) override;

		virtual const char* name() const override;
	};
}