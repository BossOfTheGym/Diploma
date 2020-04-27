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
		virtual void update(ecs::Time t, ecs::Time dt) override;


	public:
		bool startRendezvous(Entity target, Entity chaser, ecs::Time t, ecs::Time dt);

		void abortRendezvous(Entity e);

		bool rendezvousStarted(Entity e);


		// TODO : add convenient methods to push Actions
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