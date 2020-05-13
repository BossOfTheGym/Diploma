#pragma once

#include <ECS/System/System.h>
#include <ECS/Entity/Entity.h>

#include <Math/MathLib.h>


namespace sim
{
	using ecs::entity::Entity;
	using ecs::entity::null;

	using math::Vec3;

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


		// TODO : remove
		// TEST
		void startRendezvous(ecs::Time t, ecs::Time dt);

		void abortRendezvous();

		bool rendezvousStarted();

		void startLambertTransfer(const Vec3& dest, ecs::Time dt);
		// END TEST


		bool cameraBoundToNaturalAxes() const;

		void setCameraBindValue(bool bindToAxes);


	private:
		Entity m_planet{null};
		Entity m_player{null};
		Entity m_target{null};
		Entity m_chaser{null};

		bool m_paused{false};
		bool m_rendezvousStarted{false};
		bool m_bindCamToNaturalAxes{false};
		bool m_lastTimeBind{false};
	};
}