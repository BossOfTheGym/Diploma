#pragma once

#include <ECS/ecs_engine.h>
#include <ECS/Entity/Entity.h>

namespace sim
{
	using ecs::entity::Entity;
	using ecs::entity::null;
	 
	class Simulator : public ecs::ECSEngine
	{
	public:
		Simulator();

		virtual ~Simulator() = default;


	public:
		virtual void mainLoop() override;


	private:
		void init();

		void deinit();


		void loadSystems();
	};
}
