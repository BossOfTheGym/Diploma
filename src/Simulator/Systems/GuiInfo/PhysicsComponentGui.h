#pragma once

#include "GuiInfo.h"

#include "../../Components/PhysicsData.h"


namespace sim
{
	class PhysicsComponentGui : public ComponentInfo<comp::PhysicsData, PhysicsComponentGui>
	{
	public:
		using this_t = PhysicsComponentGui;
		using base_t = ComponentInfo<comp::PhysicsData, this_t>;


	public:
		PhysicsComponentGui(ecs::sys::SystemManager* manager);


	public:
		virtual void render(Entity e) override;

		const char* name() const override;
	};
}