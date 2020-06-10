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

				impuls.duration -= dt;
				if (impuls.duration.count() > 0)
				{
					return;
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

				wait.duration -= dt;
				if (wait.duration.count() > 0)
				{
					return;
				}

				sys->popFront(actionList);
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
			std::cout << "CWImpuls: bad target" << std::endl;
			sys->abortRendezvous(chaser);
			return;
		}
		auto& targetSimData = registry.get<comp::SimData>(rendComp.target);

		auto next = sys->front(chaser);
		if (!registry.valid(next) || !registry.has<comp::Action, comp::CWImpuls>(next))
		{
			// TODO : error, notify
			std::cout << "CWImpuls: failed to fetch action" << std::endl;
			sys->abortRendezvous(chaser);
			return;
		}
		auto& impuls = registry.get<comp::CWImpuls>(next);

		auto planet = simulatorState->getPlanet();
		if (!registry.valid(planet) || !registry.has<comp::Planet>(planet))
		{
			// TODO : error, notify
			std::cout << "CWImpuls: bad planet entity" << std::endl;
			sys->abortRendezvous(chaser);
			return;
		}
		auto planetComp = registry.get<comp::Planet>(planet);

		impuls.timeout -= dt;
		if (impuls.timeout.count() > 0)
		{
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
		auto newV = simData.getVelocity() + dv;
		simData.setVelocity(newV);

		// DEBUG
		auto crad = simData.getRadius();         // chaser radius
		auto cvel = simData.getVelocity();       // chaser velocity 
		auto trad = targetSimData.getRadius();   // target radius
		auto tvel = targetSimData.getVelocity(); // target velocity
		std::cout << "CWImpuls" << std::endl;
		std::cout << "    to " << impuls.timeout.count() << std::endl;
		std::cout << "    time " << ecs::toSeconds<double>(t).count() << std::endl;
		std::cout << "    dv x:" << dv.x << " y:" << dv.y << " z:" << dv.z << " mag:" << glm::length(dv) << std::endl;
		std::cout << "    cpos x: " << crad.x << " y: " << crad.y << " z: " << crad.z << std::endl;
		std::cout << "    cvel x: " << cvel.x << " y: " << cvel.y << " z: " << cvel.z << std::endl;
		std::cout << "    tpos x: " << trad.x << " y: " << trad.y << " z: " << trad.z << std::endl;
		std::cout << "    tvel x: " << tvel.x << " y: " << tvel.y << " z: " << tvel.z << std::endl;
		// end DEBUG
		simulatorState->logDvImpuls(dv, t);

		auto g0  = planetComp.g0;
		auto Isp = rendComp.Isp; 

		auto m  = physicsData.mass + rendComp.propellantMass;
		auto dm = m * (1 - std::exp(-glm::length(dv) / (g0 * Isp)));

		rendComp.propellantMass -= dm; // TODO : check for correctness
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

		auto* timeSys        = sysManager->get<TimeSystem>();
		auto* simulatorState = sysManager->get<SimulatorState>();

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
		//Vec3 i = rTarget / glm::length(rTarget);
		//Vec3 k = h / glm::length(h);
		//Vec3 j = glm::cross(k, i);
		//
		//Mat3 qi = Mat3(i, j, k);
		//Mat3 q  = glm::transpose(qi);

		//absolute
		Vec3 dr = rChaser - rTarget;
		Vec3 dv = vChaser - vTarget - glm::cross(omega, dr);

		//actions
		return -dv;
	}


	// LambertImpulsAction
	LambertImpulsAction::LambertImpulsAction(RendezvousControlSystem* sys)
		: base_t(sys)
	{}

	void LambertImpulsAction::update(Entity chaser, Time t, Time dt)
	{
		auto* sys        = getSystem();
		auto* sysManager = sys->getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();

		auto* simulatorState = sysManager->get<SimulatorState>();

		if (!registry.has<comp::PhysicsData, comp::SimData, comp::Orbit, comp::Rendezvous>(chaser))
		{
			std::cout << "LambertImpuls : bad chaser" << std::endl;
			return;
		}
		auto [physicsData, simData, orbit, rendComp] = registry.get<comp::PhysicsData, comp::SimData, comp::Orbit, comp::Rendezvous>(chaser);

		auto planet = simulatorState->getPlanet();
		if (!registry.valid(planet) || !registry.has<comp::Planet>(planet))
		{
			std::cout << "Lambert impuls: bad planet entity at simulator state" << std::endl;
			return;
		}
		auto& planetComp = registry.get<comp::Planet>(planet);

		auto next = sys->front(chaser);
		if (!registry.valid(next) || !registry.has<comp::Action, comp::LambertImpuls>(next))
		{
			std::cout << "LambertImpuls : bad action" << std::endl;
		}
		auto& lambertImpuls = registry.get<comp::LambertImpuls>(next);

		if (lambertImpuls.timeout > dt)
		{
			lambertImpuls.timeout -= dt;
			return;
		}
		lambertImpuls.timeout = Time(0);

		auto solution = space_utils::lambert::solve(
			  simData.getRadius(), ecs::toSeconds<Float>(t).count()
			, lambertImpuls.targetPosition, ecs::toSeconds<Float>(t + lambertImpuls.transferTime).count()
			, glm::cross(simData.getRadius(), simData.getVelocity())
			, planetComp.mu
			, 1
		);
		
		auto dv = solution.vel1 - simData.getVelocity();

		auto g0  = planetComp.g0;
		auto Isp = rendComp.Isp; 

		auto m  = physicsData.mass + rendComp.propellantMass;
		auto dm = m * (1 - std::exp(-glm::length(dv) / (g0 * Isp)));

		rendComp.propellantMass -= dm; // TODO : check for correctness
		rendComp.propellantUsed += dm; // TODO : check for correctness 

		simData.setVelocity(solution.vel1);

		sys->popFront(chaser);
	}
}