#pragma once

#include <ECS/System/System.h>

namespace sim
{
	class OrbitSystem : public ecs::sys::System<OrbitSystem>
	{
	public:
		using this_t = OrbitSystem;
		using base_t = ecs::sys::System<OrbitSystem>;


	public:
		OrbitSystem(ecs::sys::SystemManager* manager);


	public:
		virtual void update(ecs::Time t, ecs::Time dt) override;
	};
}