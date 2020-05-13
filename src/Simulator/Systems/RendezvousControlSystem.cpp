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

		// TODO : add lambert problem usage if it works correctly

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
		
		rendComp.propellantMass = 100.0;
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
		Vec3 dr0 = q * dr;		
		
		// actions queue
		const int SPLIT = 25;
		
		const Time FIRST_TO = Time(1'000'000); // first timeout, workaround to avoid zero time delta while update
		const Time FIRST_TR = Time(2'000'000); // first transfer interval

		Time dtPart = (dt + Time(SPLIT)) / SPLIT;		
		Time timeTransfer = dtPart * SPLIT; // total time required for transfer without first timeout
		
		// time sync
		Time timeEvent = t + FIRST_TO;

		// first
		pushBack<comp::CWImpuls>(chaser, dr0, FIRST_TO, FIRST_TR, comp::CWImpuls::First);
		timeSys->addTimeEvent(timeEvent);
		timeEvent += FIRST_TR;

		Time prevTO = FIRST_TR;
		for (int i = 1; i <= SPLIT; i++)
		{
			Vec3 pos = dr0 * (1.0_FL * (timeTransfer - i * dtPart) / timeTransfer);

			// intermediate
			pushBack<comp::CWImpuls>(chaser, pos, prevTO, dtPart, comp::CWImpuls::First);
			timeSys->addTimeEvent(timeEvent);
			timeEvent += dtPart;

			prevTO = dtPart;
		}
		// last
		pushBack<comp::CWImpuls>(chaser, Vec3{0.0}, dtPart, Time(0), comp::CWImpuls::Last);
		timeSys->addTimeEvent(timeEvent);
		 
		// init duration
		rendComp.duration = FIRST_TO + FIRST_TR + SPLIT * dtPart;

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


	// TEST
	bool RendezvousControlSystem::startLambertTransfer(Entity chaser, const Vec3& dest, ecs::Time transfer)
	{
		auto* sysManager = getSystemManager();
		auto* ecsEngine  = sysManager->getECSEngine();
		auto& registry   = ecsEngine->getRegistry();
		
		if (!registry.valid(chaser) || !registry.has<comp::Rendezvous, comp::Orbit, comp::SimData>(chaser))
		{
			// DEBUG
			std::cout << "RendezvousControl: bad chaser" << std::endl;
			return false;
		}
		auto [rendComp, orbit, simData] = registry.get<comp::Rendezvous, comp::Orbit, comp::SimData>(chaser);
		
		clear(chaser);

		rendComp.propellantMass = 1000.0;
		rendComp.propellantUsed = 0.0;
		
		pushBack<comp::LambertImpuls>(chaser, dest, Time(1'000), transfer);
		pushBack<comp::Wait>(chaser, transfer);

		rendComp.duration = Time(1'000) + transfer;

		return true;
	}
	// END TEST


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
					// TODO : also destroy action, here is destroyed only holder
					curr = next;
				}
			}
			rendezvous.target = null;
			rendezvous.actionHead = null;
			rendezvous.actionTail = null;
			rendezvous.propellantMass = 0.0;
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
				// TODO : also destroy action, here is destroyed only holder

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
}