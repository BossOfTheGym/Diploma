#include "RendezvousControlSystem.h"

#include <ECS/ecs_engine.h>
#include <ECS/System/SystemManager.h>

#include <Math/MathLib.h>

#include "../Components/Rendezvous.h"
#include "../Components/Orbit.h"
#include "../Components/SimData.h"

#include "TimeSystem.h"

// DEBUG
#include <iostream>
#include <algorithm>

namespace sim
{
	using math::operator "" _FL;


	RendezvousControlSystem::RendezvousControlSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		registerAction<ImpulsAction>();
		registerAction<WaitAction>();
		registerAction<LambertImpulsAction>();
		registerAction<CWImpulsAction>();
	}


	void RendezvousControlSystem::update(ecs::Time t, ecs::Time dt)
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
		for (auto chaser : registry.view<comp::Rendezvous>())
		{
			if (!empty(chaser))
			{
				auto& rendComp = registry.get<comp::Rendezvous>(chaser);
				rendComp.duration -= dt;	

				auto nextAction = front(chaser);
				if (registry.has<comp::Action>(nextAction))
				{
					auto& action = registry.get<comp::Action>(nextAction);
					if (auto it = m_actions.find(action.actionStaticType); it != m_actions.end())
					{
						(*it).second->update(chaser, t, dt);
					}
					else
					{
						// DEBUG
						std::cout << "Action not found" << std::endl;
					}
				}
			}
		}
	}


	bool RendezvousControlSystem::startRendezvous(Entity target, Entity chaser, ecs::Time t, ecs::Time dt)
	{
		// TODO : for now implement here some simple alg
		// TODO : logic should be definitely moved into some RendezvousStrategy 

		// TODO : far chase(interorbital transfer)

		// TODO : refactor, this looks like shit

		// close phase		
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* timeSys    = sysManager->get<TimeSystem>();

		if (!registry.valid(chaser) || !registry.has<comp::Orbit, comp::SimData, comp::Rendezvous>(chaser) ||
			!registry.valid(target) || !registry.has<comp::Orbit, comp::SimData>(target))
		{
			return false;
		}

		auto [chaserOrbit, chaserSim, rendComp] = registry.get<comp::Orbit, comp::SimData, comp::Rendezvous>(chaser);
		auto [targetOrbit, targetSim]           = registry.get<comp::Orbit, comp::SimData>(target);

		clear(chaser);
		
		// TODO :
		//rendComp.propellantMass = 100.0;
		rendComp.propellantUsed = 0.0;
		rendComp.target = target;

		//mean motion & time
		Float n = targetOrbit.getOrbit().n;

		//satellites parameters
		Vec3 rChaser = chaserSim.getRadius();
		Vec3 rTarget = targetSim.getRadius();

		Vec3 vChaser = chaserSim.getVelocity();
		Vec3 vTarget = targetSim.getVelocity();

		//angular momentum
		Vec3 h = glm::cross(rTarget, vTarget);

		//angular velocity
		Vec3 omega = h / glm::dot(rTarget, rTarget);

		//transformation matrix
		Vec3 i = rTarget / glm::length(rTarget);
		Vec3 k = h / glm::length(h);
		Vec3 j = glm::cross(k, i);

		Mat3 qi = Mat3(i, j, k);
		Mat3 q  = glm::transpose(qi);

		//absolute
		Vec3 dr = rChaser - rTarget;

		//relative
		Vec3 dr0  = q * dr;
		Float r0  = glm::length(dr0);
		Vec3 dr0u = dr0 / r0;

		// consts	
		const Time FIRST_TO    = Time(1'000'000'000); // first timeout, workaround to avoid zero & negative time delta while update
		const Time FIRST_TR    = Time(2'000'000'000); // first transfer interval

		const Time ONE_KM_TIME = Time(3'600'000'000'000); // time interval...(TODO : reasonable comment)

		const int ONE_KM_SPLIT_COUNT = 100; // split of final 1km path

		// rendezvous parameters
		Vec3 drT            = dr0; // dr travel, distance that spacecraft will travel while rendezvous
		int trajSplit       = m_split; // trajectory split
		Time transferTime   = dt; // transfer time for main path segment

		bool distLessThanKm = (r0 <= 1.0_FL);
		if (distLessThanKm)
		{
			trajSplit = ONE_KM_SPLIT_COUNT;

			transferTime = ONE_KM_TIME;
		}
		else
		{
			drT = dr0u * (r0 - 1.0_FL);
		}

		// time sync
		Time lastTransfer = FIRST_TR;

		// first impuls
		pushBack<comp::CWImpuls>(chaser, dr0, FIRST_TO, FIRST_TR, comp::CWImpuls::First);
		timeSys->addTimeEvent(t + FIRST_TO);

		for (int i = 1; i <= trajSplit; i++)
		{
			Float parammi = static_cast<Float>(i - 1) / trajSplit;
			Float parami  = static_cast<Float>(  i  ) / trajSplit;
			Float dParam = parami - parammi;

			Time dtTransfer = ecs::toTime<ecs::Tick>(dParam * transferTime);

			Vec3 pos = dr0 - parami * drT;

			// in between
			pushBack<comp::CWImpuls>(chaser, pos, lastTransfer, dtTransfer, comp::CWImpuls::First);
			timeSys->addTimeEvent(t + FIRST_TO + FIRST_TR + ecs::toTime<ecs::Tick>(transferTime * parammi));

			// store current transferTime to last transferTime
			lastTransfer = dtTransfer;
		}

		// if distance was more than 1km - we should pass this 1 km
		if (!distLessThanKm)
		{
			for (int i = 1; i <= ONE_KM_SPLIT_COUNT; i++)
			{
				Float parammi = static_cast<Float>(i - 1) / ONE_KM_SPLIT_COUNT;
				Float parami  = static_cast<Float>(  i  ) / ONE_KM_SPLIT_COUNT;
				Float dParam = parami - parammi;

				Time dtTransfer = ecs::toTime<ecs::Tick>(dParam * ONE_KM_TIME);

				Vec3 pos = dr0u * (1.0_FL - parami);

				// in between
				pushBack<comp::CWImpuls>(chaser, pos, lastTransfer, dtTransfer, comp::CWImpuls::First);
				timeSys->addTimeEvent(t + FIRST_TO + FIRST_TR + transferTime + ecs::toTime<ecs::Tick>(ONE_KM_TIME * parammi));

				// store current transferTime to last transferTime
				lastTransfer = dtTransfer;
			}
		}

		//last impuls
		pushBack<comp::CWImpuls>(chaser, Vec3{}, lastTransfer, Time{}, comp::CWImpuls::Last);
		if (distLessThanKm)
		{
			timeSys->addTimeEvent(t + FIRST_TO + FIRST_TR + transferTime);

			// duration
			rendComp.duration = FIRST_TO + FIRST_TR + transferTime;
		}
		else
		{	
			timeSys->addTimeEvent(t + FIRST_TO + FIRST_TR + transferTime + ONE_KM_TIME);

			// duration
			rendComp.duration = FIRST_TO + FIRST_TR + transferTime + ONE_KM_TIME;
		}

		return true;
	}

	void RendezvousControlSystem::abortRendezvous(Entity e)
	{
		clear(e);
	}

	bool RendezvousControlSystem::rendezvousStarted(Entity e)
	{
		return !empty(e);
	}


	void RendezvousControlSystem::clear(Entity list)
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
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
			rendezvous.target = null;
			rendezvous.actionHead = null;
			rendezvous.actionTail = null;
			// TODO
			//rendezvous.propellantMass = 0.0;
			rendezvous.propellantUsed = 0.0;
			rendezvous.duration = Time{};
		}
	}

	void RendezvousControlSystem::pushBack(Entity list, Entity action)
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
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
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
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
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
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
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);

			return rendezvous.actionTail;
		}
		return null;
	}

	Entity RendezvousControlSystem::front(Entity list)
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);

			return rendezvous.actionHead;
		}
		return null;
	}

	bool RendezvousControlSystem::empty(Entity list)
	{
		auto& registry = getSystemManager()->getECSEngine()->getRegistry();
		if (registry.has<comp::Rendezvous>(list))
		{
			auto& rendezvous = registry.get<comp::Rendezvous>(list);

			return rendezvous.actionHead == null;// && rendezvous.actionTail == null;
		}
		return false;
	}


	void RendezvousControlSystem::setSplit(int split)
	{
		m_split = split;
		m_split = std::max(std::min(m_split, 1000), 1);
	}

	int RendezvousControlSystem::getSplit() const
	{
		return m_split;
	}
}