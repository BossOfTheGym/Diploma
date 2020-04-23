#include "Actions.h"

#include <ECS/ecs_engine.h>

#include "../RendezvousControlSystem.h"
#include "../../Components/SimData.h"

namespace sim
{
	IAction::IAction(RendezvousControlSystem* sys, Id staticTypeId)
		: ecs::util::StaticType(staticTypeId)
		, m_sys(sys)
	{}

	RendezvousControlSystem* IAction::getSystem() const
	{
		return m_sys;
	}


	ImpulsAction::ImpulsAction(RendezvousControlSystem* sys)
		: BaseAction<comp::Impuls>(sys)
	{}

	void ImpulsAction::update(Entity actionList, Time t, Time dt)
	{
		auto* sys    = getSystem();
		auto* engine = sys->getSystemManager()->getECSEngine();
		auto& registry = engine->getRegistry();

		if (registry.has<comp::Rendezvous, comp::SimData>(actionList))
		{
			auto next = sys->front(actionList);
			if (next != null && registry.has<comp::Impuls>(next))
			{
				auto& impuls  = registry.get<comp::Impuls>(next);
				auto& simData = registry.get<comp::SimData>(actionList);

				simData.setVelocity(simData.getVelocity() + impuls.dv);

				sys->popFront(actionList);
			}
		}
	}


	WaitAction::WaitAction(RendezvousControlSystem* sys)
		: BaseAction<comp::Wait>(sys)
	{}

	void WaitAction::update(Entity actionList, Time t, Time dt)
	{
		auto* sys    = getSystem();
		auto* engine = sys->getSystemManager()->getECSEngine();
		auto& registry = engine->getRegistry();

		if (registry.has<comp::Rendezvous, comp::SimData>(actionList))
		{
			auto next = sys->front(actionList);
			if (next != null && registry.has<comp::Wait>(next))
			{
				auto& wait = registry.get<comp::Wait>(next);
				if (wait.duration >= dt)
				{
					wait.duration -= dt;
				}
				if (wait.duration == Time(0))
				{
					sys->popFront(actionList);
				}
			}
		}
	}
}