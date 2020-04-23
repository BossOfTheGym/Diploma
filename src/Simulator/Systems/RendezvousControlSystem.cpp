#include "RendezvousControlSystem.h"

#include <ECS/System/SystemManager.h>

#include "../Simulator.h"
#include "../Components/Rendezvous.h"



namespace sim
{
	RendezvousControlSystem::RendezvousControlSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
		, m_method(this)
	{
		m_simulator = static_cast<Simulator*>(manager->getECSEngine());

		m_actions[ImpulsAction::TYPE_ID] = std::make_unique<ImpulsAction>(this);
		m_actions[WaitAction::TYPE_ID]   = std::make_unique<WaitAction>(this);
	}


	void RendezvousControlSystem::update(ecs::Time t, ecs::Time dt)
	{
		// TODO : move to original update method
		auto& registry = m_simulator->getRegistry();
		for (auto actionList : registry.view<comp::Rendezvous>())
		{
			if (!empty(actionList))
			{
				auto nextAction = front(actionList);
				if (registry.has<comp::Action>(nextAction))
				{
					auto& action = registry.get<comp::Action>(nextAction);
					if (auto it = m_actions.find(action.actionStaticType); it != m_actions.end())
					{
						(*it).second->update(actionList, t, dt);
					}
				}
			}
		}
	}

	void RendezvousControlSystem::startRendezvous(Entity target, Entity chaser, ecs::Time t, ecs::Time dt)
	{
		m_method.startRendezvous(target, chaser, t, dt);
	}


	void RendezvousControlSystem::clear(Entity list)
	{
		auto& registry = m_simulator->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);

			auto curr = rendezvous.actionHead;
			while(curr != null)
			{
				if (registry.has<comp::Action>(curr))
				{
					auto& action = registry.get<comp::Action>(curr);

					auto next = action.nextAction;
					registry.destroy(curr);

					curr = next;
				}
			}
			rendezvous.actionHead = null;
			rendezvous.actionTail = null;
		}
	}

	void RendezvousControlSystem::pushBack(Entity list, Entity action)
	{
		auto& registry = m_simulator->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);

			if (action != null && registry.has<comp::Action>(action))
			{
				auto& act = registry.get<comp::Action>(action);
				act.nextAction = null;

				if (rendezvous.actionTail != null && registry.has<comp::Action>(rendezvous.actionTail))
				{
					auto& tail = registry.get<comp::Action>(rendezvous.actionTail);
					tail.nextAction = action;
				}
				if (rendezvous.actionTail == null)
				{
					rendezvous.actionHead = action;
				}
				rendezvous.actionTail = action;
			}
		}
	}

	void RendezvousControlSystem::pushFront(Entity list, Entity action)
	{
		auto& registry = m_simulator->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);
			
			if (action != null && registry.has<comp::Action>(action))
			{
				auto& act = registry.get<comp::Action>(action);

				act.nextAction = rendezvous.actionHead;
				rendezvous.actionHead = action;
				if (rendezvous.actionTail == null)
				{
					rendezvous.actionTail = action;
				}
			}
		}
	}

	void RendezvousControlSystem::popFront(Entity list)
	{
		auto& registry = m_simulator->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);
			if (rendezvous.actionHead != null && registry.has<comp::Action>(rendezvous.actionHead))
			{
				auto& action = registry.get<comp::Action>(rendezvous.actionHead);

				auto next = action.nextAction;
				registry.destroy(rendezvous.actionHead);

				rendezvous.actionHead = next;
				if (rendezvous.actionHead == null) // we deleted last action
				{
					rendezvous.actionTail = null;
				}
			}
		}
	}

	Entity RendezvousControlSystem::back(Entity list)
	{
		auto& registry = m_simulator->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);

			return rendezvous.actionTail;
		}
		return null;
	}

	Entity RendezvousControlSystem::front(Entity list)
	{
		auto& registry = m_simulator->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);

			return rendezvous.actionHead;
		}
		return null;
	}

	bool RendezvousControlSystem::empty(Entity list)
	{
		auto& registry = m_simulator->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);

			return rendezvous.actionHead == null;// && rendezvous.actionTail == null;
		}
		return false;
	}
}