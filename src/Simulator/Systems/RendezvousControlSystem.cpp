#include "RendezvousControlSystem.h"
#include "../Simulator.h"

#include <ECS/System/SystemManager.h>

// TODO
namespace sim
{
	RendezvousControlSystem::RendezvousControlSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		m_simulator = static_cast<Simulator*>(manager->getECSEngine());
	}

	void RendezvousControlSystem::update(ecs::Float t, ecs::Float dt)
	{
		
	}
}