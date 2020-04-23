#pragma once

#include <Math/MathLib.h>

#include <ECS/Entity/Entity.h>

#include "../../Components/Timer.h"

namespace sim
{
	using ecs::entity::Entity;
	using ecs::Time;

	class RendezvousControlSystem;

	class Method
	{
	public:
		Method(RendezvousControlSystem* sys);

		virtual ~Method() = default;


	public:
		virtual bool startRendezvous(Entity target, Entity chaser, Time t, Time dt) = 0;


	public:
		RendezvousControlSystem* getSystem() const;


	private:
		RendezvousControlSystem* m_sys{nullptr};
	};
}