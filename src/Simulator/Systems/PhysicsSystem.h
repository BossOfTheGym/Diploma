#pragma once

#include <ECS/System/System.h>

namespace sim
{
	class Simulator;

	// updates rotation
	// position and velocity is updated by PlanetSystem
	class PhysicsSystem final : public ecs::sys::System<PhysicsSystem>
	{
	public:
		PhysicsSystem(ecs::sys::SystemManager* manager);

		virtual ~PhysicsSystem() = default;

	public:
		virtual void update(ecs::Time t, ecs::Time dt) override;

	private:
		Simulator* m_simulator;
	};
}