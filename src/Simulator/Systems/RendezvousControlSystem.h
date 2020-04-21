#pragma once

#include <ECS/Entity/Entity.h>
#include <ECS/System/System.h>

#include "Rendezvous/Actions.h"
#include "Rendezvous/Circular.h"

#include <unordered_map>
#include <memory>

namespace sim
{
	class Simulator;

	class RendezvousControlSystem 
		: public ecs::sys::System<RendezvousControlSystem>
	{
	public:
		using this_t = RendezvousControlSystem;
		using base_t = ecs::sys::System<RendezvousControlSystem>;

		using ActionPtr      = std::unique_ptr<IAction>;
		using ActionRegistry = std::unordered_map<Id, ActionPtr>;
		

	public:
		RendezvousControlSystem(ecs::sys::SystemManager* manager);

		virtual ~RendezvousControlSystem() = default;


	public:
		// TODO
		virtual void update(ecs::Float t, ecs::Float dt) override;

		// TODO : total workaround, replace ecs::Float to duration<>(see appropriate ticket)
		// TODO : to do
		void update(Tick ticks);


	public:
		// TODO : workaround
		void startRendezvous(Entity e1, Entity e2, Tick ticks, Float ticksF);


		// TODO : that's weird
		// works with Rendezvous component
		void clear(Entity list);

		void pushBack(Entity list, Entity e);

		void pushFront(Entity list, Entity e);

		void popFront(Entity list);


		Entity back(Entity list);

		Entity front(Entity list);

		bool empty(Entity list);


	private:
		Simulator* m_simulator{nullptr};	

		ActionRegistry m_actions{};

		// TODO
		ToCircular m_method;
	};
}