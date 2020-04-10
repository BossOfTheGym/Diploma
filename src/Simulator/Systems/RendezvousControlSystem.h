#pragma once

#include <ECS/Entity/Entity.h>
#include <ECS/System/System.h>

// TODO
namespace sim
{
	class Simulator;

	class RendezvousControlSystem 
		: public ecs::sys::System<RendezvousControlSystem>
	{
	public:
		using this_t = RendezvousControlSystem;
		using base_t = ecs::sys::System<RendezvousControlSystem>;


	public:
		RendezvousControlSystem(ecs::sys::SystemManager* manager);

		virtual ~RendezvousControlSystem() = default;


	public:
		virtual void update(ecs::Float t, ecs::Float dt) override;


	private:
		Simulator* m_simulator{nullptr};	
	};
}