#include "PhysicsSystem.h"

#include "../Simulator.h"
#include "../Components/PhysicsData.h"
#include "../Components/Transform.h"

#include <Math/MathLib.h>

#include <iostream>

namespace sim
{
	using comp::PhysicsData;
	using comp::Transform;

	using math::Vec3f32;
	using math::Quatf32;

	PhysicsSystem::PhysicsSystem(ecs::sys::SystemManager* manager)
		: ecs::sys::System<PhysicsSystem>(manager)
	{
		m_simulator = static_cast<Simulator*>(manager->getECSEngine());
		// TODO : assert
		// TODO : maybe dynamic_cast better?
	}

	void PhysicsSystem::update(ecs::Float t, ecs::Float dt)
	{
		auto& registry = m_simulator->getRegistry();

		auto view = registry.view<Transform, PhysicsData>();
		for (auto& e : view)
		{
			auto& transform = registry.get<Transform>(e);
			auto& physics   = registry.get<PhysicsData>(e);

			auto dq = glm::angleAxis(physics.angle * dt, physics.axis);
			
			transform.rotation  = Quatf32(dq) * transform.rotation;
			transform.translate = Vec3f32(physics.r);
		}
	}
}