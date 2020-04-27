#pragma once

#include "GuiInfo.h"

#include "../../Components/Orbit.h"

namespace sim
{
	class OrbitComponentGui : public ComponentInfo<comp::Orbit, OrbitComponentGui>
	{
	public: 
		using this_t = OrbitComponentGui;
		using base_t = ComponentInfo<comp::Orbit, OrbitComponentGui>;


	public:
		OrbitComponentGui(ecs::sys::SystemManager* manager);


	public:
		virtual void render(Entity e) override;

		virtual const char* name() const override;
	};
}