#pragma once

#include <Context/Context.h>
#include <ECS/System/System.h>
#include <ECS/Entity/Entity.h>

namespace sim
{
	using context::Hint;
	using context::Hints;
	using context::CreationInfo;
	using context::BaseWindow;
	using context::IMediator;

	using ecs::entity::Entity;
	using ecs::entity::null;


	class Simulator;

	// TODO : move out input functionality to InputSystem
	class ContextSystem : public ecs::sys::System<ContextSystem>, public BaseWindow
	{		
	public:
		using this_t = ContextSystem;
		using base_t1 = ecs::sys::System<ContextSystem>;
		using base_t2 = BaseWindow;


	public:
		ContextSystem(ecs::sys::SystemManager* manager, const CreationInfo& info = {}, bool initGL = true);

		virtual ~ContextSystem();


	public: // System<T>
		virtual void update(ecs::Float t, ecs::Float dt) override;


	public: //BaseWindow
		virtual void mouseEvent(double xPos, double yPos) override;

		virtual void mouseButtonEvent(int button, int action, int mods) override;

		virtual void scrollEvent(double xOffset, double yOffset) override;

		virtual void keyEvent(int key, int scancode, int action, int mods) override;


	private:
		Simulator* m_simulator{nullptr};
		Entity m_controlled{null};
	};
}