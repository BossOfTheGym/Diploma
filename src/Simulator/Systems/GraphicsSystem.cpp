#include "GraphicsSystem.h"


namespace sim
{
	using GraphicsEngine = GraphicsSystem::GraphicsEngine;

	GraphicsSystem::GraphicsSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{}

	GraphicsEngine& GraphicsSystem::getGraphicsEngine()
	{
		return m_engine;
	}
}