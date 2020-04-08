#pragma once

#include <ECS/ecs_engine.h>

namespace sim
{
	class ContextSystem;
	class TimeSystem;

	class Simulator : public ecs::ECSEngine
	{
	public:
		Simulator();

		virtual ~Simulator() = default;


	public:
		virtual void mainLoop() override;
	};
}
