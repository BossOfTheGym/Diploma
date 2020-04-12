#pragma once

#include <ECS/System/System.h>
#include <ECS/Entity/Entity.h>

#include "InputController/InputController.h"

#include <memory>

namespace sim
{
	using ecs::entity::Entity;
	using ecs::entity::null;

	
	class Simulator;

	class PlayerSystem : public ecs::sys::System<PlayerSystem>
	{
	public:
		using this_t = PlayerSystem;
		using base_t = ecs::sys::System<PlayerSystem>;

		using InputControllerRaw = InputController*;
		using InputControllerPtr = std::unique_ptr<InputController>;


	public:
		PlayerSystem(ecs::sys::SystemManager* manager);

		virtual ~PlayerSystem() = default;


	public:
		virtual void update(ecs::Float t, ecs::Float dt) override;


	public:
		// TODO : camera manipulations & InputController
		InputController* getInputController() const;


	private:
		Simulator* m_simulator{nullptr};

		InputControllerPtr m_controller{nullptr};
	};
}