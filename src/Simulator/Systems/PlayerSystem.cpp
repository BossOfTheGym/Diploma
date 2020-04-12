#include "PlayerSystem.h"

#include "InputController/InputController.h"

#include "../Components/Camera.h"
#include "../Components/Player.h"
#include "../Components/Transform.h"
#include "../Simulator.h"

// TODO
namespace sim
{
	PlayerSystem::PlayerSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		// TODO : assert
		m_simulator = static_cast<Simulator*>(manager->getECSEngine());

		m_controller = std::make_unique<InputController>(m_simulator);
	}


	void PlayerSystem::update(ecs::Float t, ecs::Float dt)
	{
		auto& registry = m_simulator->getRegistry();
		for (auto e : registry.view<comp::Player, comp::Camera3rdPerson>())
		{
			auto [player, camera] = registry.get<comp::Player, comp::Camera3rdPerson>(e);

			if (player.view != e && registry.valid(player.view) && registry.has<comp::Transform>(player.view))
			{
				auto& transform = registry.get<comp::Transform>(player.view);

				camera.setCenter(transform.translate);
			}
		}
	}


	InputController* PlayerSystem::getInputController() const
	{
		return m_controller.get();
	}
}