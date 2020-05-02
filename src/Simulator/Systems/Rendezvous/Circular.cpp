#include "Circular.h"

#include <Math/MathLib.h>

#include <ECS/System/SystemManager.h>
#include <ECS/ecs_engine.h>

#include <SpaceUtils/ClohessyWiltshire.h>
#include <SpaceUtils/Lambert.h>
#include <SpaceUtils/StateVec.h>

#include <iostream>

#include "Actions.h"

#include "../RendezvousControlSystem.h"
#include "../TimeSystem.h"

#include "../../Components/Rendezvous.h"
#include "../../Components/Orbit.h"
#include "../../Components/SimData.h"

namespace sim
{
	using math::Float;
	using math::Vec3;
	using math::Mat3;


	ToCircular::ToCircular(RendezvousControlSystem* sys) 
		: TwoImpulsMethod(sys)
	{}

	auto ToCircular::startRendezvous(Entity target, Entity chaser, Time t, Time dt) -> Data
	{
		// TODO : check for appropriate eccentricity
		// TODO : far-stage & interorbital transition
		auto* sys        = getSystem();
		auto* sysManager = sys->getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto& registry   = engine->getRegistry();
		auto* timeSys = sysManager->get<TimeSystem>();

		if (registry.has<comp::Orbit, comp::SimData>(target)
			&& registry.has<comp::Orbit, comp::SimData, comp::Rendezvous>(chaser))
		{
			auto [targetOrbit, targetSim]                 = registry.get<comp::Orbit, comp::SimData>(target);
			auto [chaserOrbit, chaserSim, chaserRendComp] = registry.get<comp::Orbit, comp::SimData, comp::Rendezvous>(chaser);

			sys->abortRendezvous(chaser);
			chaserRendComp.duration = dt;

			//mean motion & time
			Float n = targetOrbit.getOrbit().n;
			Float T = ecs::toSeconds<Float>(t).count(); // transfer time

			//satellites parameters
			Vec3 rChaser = chaserSim.getRadius();
			Vec3 rTarget = targetSim.getRadius();

			Vec3 vChaser = chaserSim.getVelocity();
			Vec3 vTarget = targetSim.getVelocity();

			//energy
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


			Vec3 dvNew0 = -glm::inverse(space_utils::phi_rv(T, n)) * space_utils::phi_rr(T, n) * dr0;
			Vec3 dvNew1 = space_utils::phi_vr(T, n) * dr0 + space_utils::phi_vv(T, n) * dvNew0;

			//actions
			Vec3 dvFirstBefore = (dvNew0 - dv0);
			Vec3 dvFirst  = qi * dvFirstBefore;

			Vec3 dvSecondBefore = (-dvNew1); 
			Vec3 dvSecond = qi * dvSecondBefore;

			// DEBUG
			//std::cout << "First : x:" << dvFirst.x  << " y: " << dvFirst.y  << " z: " << dvFirst.z  << std::endl;
			//std::cout << "Second: x:" << dvSecond.x << " y: " << dvSecond.y << " z: " << dvSecond.z << std::endl;

			// TODO : add to rendezvous control system convenient methods to push actions
			// first impuls
			//auto firstImpuls = registry.create();
			//registry.assign<comp::Action>(firstImpuls, null, comp::Impuls::TYPE_ID);
			//registry.assign<comp::Impuls>(firstImpuls, dvFirst, Time(0));
			//sys->pushBack(chaser, firstImpuls);

			// wait
			//auto wait = registry.create();
			//registry.assign<comp::Action>(wait, null, comp::Wait::TYPE_ID);
			//registry.assign<comp::Wait>(wait, dt);
			//sys->pushBack(chaser, wait);

			// second impuls
			///auto secondImpuls = registry.create();
			///registry.assign<comp::Action>(secondImpuls, null, comp::Impuls::TYPE_ID);
			///registry.assign<comp::Impuls>(secondImpuls, dvSecond, dt);
			///sys->pushBack(chaser, secondImpuls);

			// TODO : check
			//timeSys->addTimeEvent(t);
			//timeSys->addTimeEvent(t + dt);

			return {true, dvFirst, dvSecond};
		}
		return {false, {}, {}};
	}
}