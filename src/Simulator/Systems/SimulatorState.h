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
		virtual void update(ecs::Time t, ecs::Time dt) override;


	public:
		Entity getPlanet() const;

		Entity getPlayer() const;

		Entity getTarget() const;

		Entity getChaser() const;


		bool paused() const;

		void pause();

		void resume();


		// TEST
		void startRendezvous(ecs::Time t, ecs::Time dt);

		void abortRendezvous();

		bool rendezvousStarted();
		// END TEST


	private:
		Entity m_planet{null};
		Entity m_player{null};
		Entity m_target{null};
		Entity m_chaser{null};

		bool m_paused{false};
		bool m_rendezvousStarted{false};
	};
}