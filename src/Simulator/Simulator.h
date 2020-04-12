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

		void loadEntityBuilders();	
		

	public:
		Entity getPlanet() const;

		Entity getPlayer() const;

		Entity getSat1() const;

		Entity getSat2() const;


	private:
		Entity m_planet{null};
		Entity m_player{null};
		Entity m_sat1{null};
		Entity m_sat2{null};

		bool m_paused{false};
	};
}
