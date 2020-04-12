#include "InputController.h"

#include "../../Components/Camera.h"
#include "../../Simulator.h"

namespace sim
{
	InputController::InputController(Simulator* host, Entity controlled) 
		: IInputController(host)
	{
		// TODO : assert
		m_simulator  = host;
		m_controlled = controlled;
	}
	

	void InputController::mouseEvent(BaseWindow& window, double xPos, double yPos)
	{
		if (m_pressed)
		{
			auto& registry = m_simulator->getRegistry();
			if (registry.valid(m_controlled) && registry.has<comp::Camera3rdPerson>(m_controlled))
			{
				auto& camera = registry.get<comp::Camera3rdPerson>(m_controlled);
				camera.rotateAround(2 * math::PI * (xPos - m_prevX) / window.getWindowSize().x);
				camera.rotatePitch (2 * math::PI * (yPos - m_prevY) / window.getWindowSize().x);
			}
		}
		m_prevX = xPos;
		m_prevY = yPos;
		
	}

	void InputController::mouseButtonEvent(BaseWindow & window, int button, int action, int mods)
	{		
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			m_pressed = (action == GLFW_PRESS);
		}
	}

	void InputController::scrollEvent(BaseWindow & window, double xOffset, double yOffset)
	{
		auto& registry = m_simulator->getRegistry();
		if (registry.valid(m_controlled) && registry.has<comp::Camera3rdPerson>(m_controlled))
		{
			auto& camera = registry.get<comp::Camera3rdPerson>(m_controlled);
			camera.changeDistance(yOffset / 5);
		}
	}

	void InputController::keyEvent(BaseWindow & window, int key, int scancode, int action, int mods)
	{}


	void InputController::setControlledEntity(Entity e)
	{
		m_controlled = e;
	}

	Entity InputController::getControlledEntity() const
	{
		return m_controlled;
	}
}