#pragma once

#include <ECS/Entity/Entity.h>
#include <ECS/System/System.h>

#include "../Components/Rendezvous.h"

#include "Rendezvous/Actions.h"

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

		

		template<class Action, class ... Args>
		void registerAction(Args&& ... args)
		{
			m_actions[Action::TYPE_ID] = std::make_unique<Action>(this, std::forward<Args>(args)...);
		}

		template<class Action>
		void unregisterAction()
		{
			m_actions.erase(Action::TYPE_ID);
		}


		template<class ActionComponent, class ... Args>
		void pushBack(Entity chaser, Args&& ... args)
		{
			auto& registry = getSystemManager()->getECSEngine()->getRegistry();

			auto action = registry.create();
			registry.assign<comp::Action>(action, null, ActionComponent::TYPE_ID);
			registry.assign<ActionComponent>(action, std::forward<Args>(args)...);

			pushBack(chaser, action);
		};

		template<class ActionComponent, class ... Args>
		void pushFront(Entity chaser, Args&& ... args)
		{
			auto& registry = getSystemManager()->getECSEngine()->getRegistry();

			auto action = registry.create();
			registry.assign<comp::Action>(action, null, ActionComponent::TYPE_ID);
			registry.assign<ActionComponent>(action, std::forward<Args>(args)...);

			pushFront(chaser, action);
		}


		// list entity should have Rendezvous component(it serves as holder of head and tail of a list)
		// TODO : maybe there's some better way to work with list
		void clear(Entity chaser);

		void pushBack(Entity chaser, Entity action);

		void pushFront(Entity chaser, Entity action);

		void popFront(Entity chaser);


		Entity back(Entity chaser);

		Entity front(Entity chaser);

		bool empty(Entity chaser);


	private:
		ActionRegistry m_actions{};
	};
}