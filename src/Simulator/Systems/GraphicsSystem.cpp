#include "GraphicsSystem.h"


namespace sim
{
	using GraphicsEngine = GraphicsSystem::GraphicsEngine;

	GraphicsSystem::GraphicsSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		auto e = glGetError();
	}

	GraphicsEngine& GraphicsSystem::getGraphicsEngine()
	{
		return m_engine;
	}
}