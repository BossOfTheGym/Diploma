#include "ContextSystem.h"

#include <utility>
#include <glWrap/OpenGL.h>

#include "../Components/Camera.h"

#include "../Simulator.h"

namespace sim
{
	ContextSystem::ContextSystem(ecs::sys::SystemManager* manager, const CreationInfo& info, bool initializeGL)
		: base_t1(manager)
		, base_t2(info, initializeGL)
	{
		// TODO : add assert
		context::initialize();

		m_simulator = static_cast<Simulator*>(manager->getECSEngine());
	}

	ContextSystem::~ContextSystem()
	{
		context::terminate();
	}

	void ContextSystem::update(ecs::Float t, ecs::Float dt)
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
}