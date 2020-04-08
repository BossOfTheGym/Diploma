#pragma once

#include <ECS/ecs_common.h>
#include <ECS/System/System.h>

#include <GraphicsEngine/GraphicsEngine.h>

namespace sim
{
	class GraphicsSystem : public ecs::sys::System<GraphicsSystem>
	{
	public:
		using this_t = GraphicsSystem;
		using base_t = ecs::sys::System<GraphicsSystem>;

		using GraphicsEngine = graphics::GraphicsEngine;


	public:
		GraphicsSystem(ecs::sys::SystemManager* manager);

		virtual ~GraphicsSystem() = default;


	public:
		GraphicsEngine& getGraphicsEngine();


	private:
		GraphicsEngine m_engine{};
	};
}