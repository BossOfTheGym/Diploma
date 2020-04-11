#include "ContextSystem.h"

#include <utility>
#include <glWrap/OpenGL.h>

#include "../Components/Camera.h"

#include "../Simulator.h"

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

	ContextSystem::~ContextSystem()
	{}

	void ContextSystem::pollEvents()
	{
		context::pollEvents();
	}


	void ContextSystem::mouseEvent(double xPos, double yPos)
	{
		//m_host->getCamera().rotateAround(2 * math::PI * (xPos - m_prevX) / window.getWindowSize().x);
		//m_host->getCamera().rotatePitch(2 * math::PI * (yPos - m_prevY) /window.getWindowSize().x);
		//m_prevX = xPos;
		//m_prevY = yPos;
	}

	void ContextSystem::mouseButtonEvent(int button, int action, int mods)
	{
	}

	void ContextSystem::scrollEvent(double xOffset, double yOffset)
	{
		//m_host->getCamera().changeDistance(yOffset / 100);
	}

	void ContextSystem::keyEvent(int key, int scancode, int action, int mods)
	{
	}

	const Mat4f32& ContextSystem::getProjection() const
	{
		return m_projection;
	}
}