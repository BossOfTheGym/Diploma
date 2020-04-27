#pragma once

#include "GuiInfo.h"
#include "../../Components/Planet.h"

namespace sim
{
	class PlanetComponentGui : public ComponentInfo<comp::Planet, PlanetComponentGui>
	{
	public:
		using this_t = PlanetComponentGui;
		using base_t = ComponentInfo<comp::Planet, PlanetComponentGui>;


	public:
		PlanetComponentGui(ecs::sys::SystemManager* manager);


	public:
		virtual void render(Entity e) override;

		virtual const char* name() const override;
	};
}