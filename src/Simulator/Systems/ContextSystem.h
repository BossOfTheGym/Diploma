#pragma once

#include <Context/Context.h>
#include <ECS/System/System.h>
#include <ECS/Entity/Entity.h>

#include <Math/MathLib.h>

namespace sim
{
	using context::Hint;
	using context::Hints;
	using context::CreationInfo;
	using context::BaseWindow;
	using context::IMediator;

	using ecs::entity::Entity;
	using ecs::entity::null;

	using math::Float;
	using math::Mat4f32;


	class Simulator;

	// TODO : move out input functionality to InputSystem
	class ContextSystem : public ecs::sys::System<ContextSystem>, public BaseWindow
	{		
	public:
		using this_t = ContextSystem;
		using base_t1 = ecs::sys::System<ContextSystem>;
		using base_t2 = BaseWindow;


	public:
		ContextSystem(
			ecs::sys::SystemManager* manager
			, const CreationInfo& info = {}
			, bool initGL = true
			, Float fovy = glm::radians(45.0), Float near = 1.0, Float far = 1000.0
		);

		virtual ~ContextSystem();


	public:
		void pollEvents();


	public: //BaseWindow
		virtual void mouseEvent(double xPos, double yPos) override;

		virtual void mouseButtonEvent(int button, int action, int mods) override;

		virtual void scrollEvent(double xOffset, double yOffset) override;

		virtual void keyEvent(int key, int scancode, int action, int mods) override;


	public:
		const Mat4f32& getProjection() const;


	private:
		Simulator* m_simulator{nullptr};		

		Mat4f32 m_projection{};
	};
}