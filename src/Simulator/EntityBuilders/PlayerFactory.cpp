#include "PlayerFactory.h"

#include <ECS/Entity/EntityBuilderManager.h>
#include <Math/MathLib.h>

#include "../Components/Player.h"
#include "../Components/Camera.h"
#include "../Simulator.h"

// TODO
namespace sim
{
	using math::Vec3f32;

	PlayerFactory::PlayerFactory(ecs::entity::EntityBuilderManager* manager)
		: base_t(manager)
	{
		// TODO : add assert
		m_simulator = static_cast<Simulator*>(manager->getEngineInstance());
	}

	Entity PlayerFactory::buildEntity()
	{
		auto& registry = m_simulator->getRegistry();

		auto player = registry.create();
		registry.assign<comp::Player>         (player, player);
		registry.assign<comp::Camera3rdPerson>(player, Vec3f32{}, math::PI_3, 0.0, 0.0, 1.0, 1.0, 500.0);
		return player;
	}
}
