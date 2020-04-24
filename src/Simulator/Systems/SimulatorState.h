#pragma once

#include <ECS/System/System.h>
#include <ECS/Entity/Entity.h>

namespace sim
{
	using ecs::entity::Entity;
	using ecs::entity::null;

	class SimulatorState : public ecs::sys::System<SimulatorState>
	{
	public:
		using this_t = SimulatorState;
		using base_t = ecs::sys::System<SimulatorState>;


	public:
		SimulatorState(ecs::sys::SystemManager* manager);


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

		bool m_paused{ false };
	};
}