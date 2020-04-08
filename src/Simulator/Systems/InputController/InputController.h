#pragma once

#include "IInputController.h"

#include <ECS/Entity/Entity.h>
#include <Math/MathLib.h>

namespace sim
{
	using math::Float;
	using ecs::entity::Entity;
	using ecs::entity::null;

	class Simulator;

	class InputController : public IInputController
	{
	public:
		InputController(Simulator* host);

		virtual ~InputController() = default;


	public: // Inherited via IInputController
		virtual void mouseEvent(BaseWindow& window, double xPos, double yPos) override;

		virtual void mouseButtonEvent(BaseWindow& window, int button, int action, int mods) override;

		virtual void scrollEvent(BaseWindow& window, double xOffset, double yOffset) override;

		virtual void keyEvent(BaseWindow& window, int key, int scancode, int action, int mods) override;


	public:
		

	private:
		Simulator* m_simulator{nullptr};

		Entity m_controlled{null};

		Float m_prevX{};
		Float m_prevY{};
	};
}