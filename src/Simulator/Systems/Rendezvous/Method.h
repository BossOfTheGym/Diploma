#pragma once

#include <Math/MathLib.h>

#include <ECS/Entity/Entity.h>

#include "../../Components/Timer.h"

namespace sim
{
	using ecs::entity::Entity;
	using comp::Tick;
	using math::Float;

	class RendezvousControlSystem;

	class Method
	{
	public:
		Method(RendezvousControlSystem* sys);

		virtual ~Method() = default;


	public:
		// TODO : timeF is workaround, duration, duration and duration
		virtual bool startRendezvous(Entity target, Entity chaser, Tick time, Float timeF) = 0;


	public:
		RendezvousControlSystem* getSystem() const;


	private:
		RendezvousControlSystem* m_sys{nullptr};
	};
}