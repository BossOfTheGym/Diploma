#pragma once

#include "GuiInfo.h"

#include "../../Components/Rendezvous.h"

namespace sim
{
	class RendezvousComponentGui : public ComponentInfo<comp::Rendezvous, RendezvousComponentGui>
	{
	public:
		using this_t = RendezvousComponentGui;
		using base_t = ComponentInfo<comp::Rendezvous, this_t>;


	public:
		RendezvousComponentGui(ecs::sys::SystemManager* manager);


	public:
		virtual void render(Entity e) override;

		virtual const char* name() const override;
	};
}