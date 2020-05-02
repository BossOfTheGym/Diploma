#include "Actions.h"

#include <ECS/ecs_engine.h>

#include <Math/MathLib.h>

#include <SpaceUtils/ClohessyWiltshire.h>
#include <SpaceUtils/Lambert.h>
#include <SpaceUtils/StateVec.h>

#include "../RendezvousControlSystem.h"
#include "../TimeSystem.h"
#include "../SimulatorState.h"

#include "../../Components/SimData.h"
#include "../../Components/PhysicsData.h"
#include "../../Components/Orbit.h"
#include "../../Components/Planet.h"

// DEBUG
#include <iostream>

namespace sim
{
	// IAction
	IAction::IAction(RendezvousControlSystem* sys, Id staticTypeId)
		: ecs::util::StaticType(staticTypeId)
		, m_sys(sys)
	{}

	RendezvousControlSystem* IAction::getSystem() const
	{
		return m_sys;
	}


	// ImpulsAction, not used
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

				if (impuls.duration >= dt)
				{
					impuls.duration -= dt;
				}
				if (impuls.duration == Time(0))
				{
					sys->popFront(actionList);
				}

				simData.setVelocity(simData.getVelocity() + impuls.dv);

				sys->popFront(actionList);
			}
		}
	}


	// WaitAction, not used
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


	// CWImpulsAction, used
	CWImpulsAction::CWImpulsAction(RendezvousControlSystem* sys)
		: base_t(sys)
	{}

	void CWImpulsAction::update(Entity chaser, Time t, Time dt)
	{
		auto* sys        = getSystem();
		auto* sysManager = sys->getSystemManager();
		auto* engine     = sys->getSystemManager()->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* simulatorState = sysManager->get<SimulatorState>();

		if (!registry.has<comp::Rendezvous, comp::SimData, comp::PhysicsData>(chaser))
		{
			std::cout << "CWImpuls: bad chaser" << std::endl;
			sys->abortRendezvous(chaser);
			return;
		}
		auto [rendComp, simData, physicsData] = registry.get<comp::Rendezvous, comp::SimData, comp::PhysicsData>(chaser);

		if (!registry.valid(rendComp.target) || !registry.has<comp::Orbit, comp::SimData>(rendComp.target))
		{
			// TODO : in fact not an error but still should notify
			std::cout << "CWImpuls: bad target" << std::endl;
			sys->abortRendezvous(chaser);
			return;
		}

		auto next = sys->front(chaser);
		if (!registry.valid(next) || !registry.has<comp::CWImpuls>(next))
		{
			// TODO : error, notify
			std::cout << "CWImpuls: failed to fetch action" << std::endl;
			sys->abortRendezvous(chaser);
			return;
		}

		auto planet = simulatorState->getPlanet();
		if (!registry.valid(planet) || !registry.has<comp::Planet>(planet))
		{
			// TODO : error, notify
			std::cout << "CWImpuls: bad planet entity" << std::endl;
			sys->abortRendezvous(chaser);
			return;
		}
		auto planetComp = registry.get<comp::Planet>(planet);

		auto& impuls = registry.get<comp::CWImpuls>(next);
		if (impuls.timeout >= dt)
		{
			impuls.timeout -= dt;
			return;
		}

		Vec3 dv{};
		switch(impuls.impulsType)
		{
			case comp::CWImpuls::First:
			{
				dv = computeFirstImpulse(chaser, impuls);
				break;
			}

			case comp::CWImpuls::Last:
			{
				dv = computeSecondImpulse(chaser, impuls);	
				break;
			}
		}
		simData.setVelocity(simData.getVelocity() + dv);

		std::cout << "CWImpuls: impuls " << dv.x << " " << dv.y << " " << dv.z << std::endl;

		auto g0  = planetComp.g0;
		auto Isp = rendComp.Isp; 

		auto m  = physicsData.mass + rendComp.propellantMass;
		auto dm = m * (1 - std::exp(-glm::length(dv) / (g0 * Isp)));

		rendComp.propellantUsed += dm; // TODO : check for correctness 

		sys->popFront(chaser);
	}

	// private
	Vec3 CWImpulsAction::computeFirstImpulse(Entity chaser, const comp::CWImpuls& impuls)
	{
		auto* sys        = getSystem();
		auto* sysManager = sys->getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();
		auto* timeSys    = sysManager->get<TimeSystem>();


		auto [chaserOrbit, chaserSim, rendComp] = registry.get<comp::Orbit, comp::SimData, comp::Rendezvous>(chaser);
		auto [targetOrbit, targetSim]           = registry.get<comp::Orbit, comp::SimData>(rendComp.target);


		//mean motion & time
		Float n = targetOrbit.getOrbit().n;
		Float T = ecs::toSeconds<Float>(impuls.transferTime).count(); // transfer time

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
		Vec3 dv = vChaser - vTarget - glm::cross(omega, dr);

		//relative
		Vec3 dr0 = q * dr;
		Vec3 dv0 = q * dv;

		Vec3 dvNew0 = glm::inverse(space_utils::phi_rv(T, n)) * (impuls.targetPosition - space_utils::phi_rr(T, n) * dr0);

		//actions
		return qi * (dvNew0 - dv0);
	}

	Vec3 CWImpulsAction::computeSecondImpulse(Entity chaser, const comp::CWImpuls& impuls)
	{
		auto* sys        = getSystem();
		auto* sysManager = sys->getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();
		auto* timeSys    = sysManager->get<TimeSystem>();


		auto [chaserOrbit, chaserSim, rendComp] = registry.get<comp::Orbit, comp::SimData, comp::Rendezvous>(chaser);
		auto [targetOrbit, targetSim]           = registry.get<comp::Orbit, comp::SimData>(rendComp.target);


		//mean motion & time
		Float n = targetOrbit.getOrbit().n;
		Float T = ecs::toSeconds<Float>(impuls.transferTime).count(); // transfer time

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
		Vec3 dv = vChaser - vTarget - glm::cross(omega, dr);

		//actions
		return -dv;
	}
}