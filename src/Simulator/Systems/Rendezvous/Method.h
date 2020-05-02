#pragma once

#include <Math/MathLib.h>

#include <ECS/Entity/Entity.h>

#include "../../Components/Timer.h"

// not used

namespace sim
{
	using ecs::entity::Entity;
	using ecs::Time;

	using math::Vec3;


	class RendezvousControlSystem;

	class TwoImpulsMethod
	{
	public:
		struct Data
		{
			bool started{};

			Vec3 dv1{};
			Vec3 dv2{};
		};


	public:
		TwoImpulsMethod(RendezvousControlSystem* sys);

		virtual ~TwoImpulsMethod() = default;


	public:
		virtual Data startRendezvous(Entity target, Entity chaser, Time t, Time dt) = 0;


	public:
		RendezvousControlSystem* getSystem() const;


	private:
		RendezvousControlSystem* m_sys{nullptr};
	};
}