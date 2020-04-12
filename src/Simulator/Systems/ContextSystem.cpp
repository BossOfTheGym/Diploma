#include "ContextSystem.h"

#include "InputController/IInputController.h"

#include "../Components/Camera.h"
#include "../Simulator.h"

#include <utility>


namespace sim
{
	ContextSystem::ContextSystem(
		  ecs::sys::SystemManager* manager
		, const CreationInfo& info
		, bool initializeGL
		, Float fovy, Float near, Float far
	)
		: base_t1(manager)
		, base_t2(info, initializeGL)
	{
		// TODO : add assert
		m_simulator = static_cast<Simulator*>(manager->getECSEngine());

		auto size = getWindowSize();
		m_projection = glm::perspective(fovy, 1.0 * size.x / size.y, near, far);
	}


	void ContextSystem::pollEvents()
	{
		context::pollEvents();
	}

	void ContextSystem::setSwapInterval(int interval)
	{
		context::setSwapInterval(interval);
	}


	void ContextSystem::mouseEvent(double xPos, double yPos)
	{
		if (!m_controllers.empty())
		{
			m_controllers.back()->mouseEvent(*this, xPos, yPos);
		}
	}

	void ContextSystem::mouseButtonEvent(int button, int action, int mods)
	{
		if (!m_controllers.empty())
		{
			m_controllers.back()->mouseButtonEvent(*this, button, action, mods);
		}
	}

	void ContextSystem::scrollEvent(double xOffset, double yOffset)
	{
		if (!m_controllers.empty())
		{
			m_controllers.back()->scrollEvent(*this, xOffset, yOffset);
		}
	}

	void ContextSystem::keyEvent(int key, int scancode, int action, int mods)
	{
		if (!m_controllers.empty())
		{
			m_controllers.back()->keyEvent(*this, key, scancode, action, mods);
		}
	}


	void ContextSystem::pushController(IInputController* controller)
	{
		m_controllers.push_back(controller);
	}

	IInputController* ContextSystem::peekController()
	{
		return (!m_controllers.empty() ? m_controllers.back() : nullptr);
	}

	void ContextSystem::popController()
	{
		if (!m_controllers.empty())
		{
			m_controllers.pop_back();
		}
	}


	const Mat4f32& ContextSystem::getProjection() const
	{
		return m_projection;
	}
}