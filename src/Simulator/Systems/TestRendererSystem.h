#pragma once

#include <ECS/System/System.h>

#include <glWrap/Shader/Shader.h>
#include <glWrap/Shader/ShaderProgram.h>


namespace sim
{
	class Simulator;

	// TODO : add IRenderer class
	// virtual setREnderState
	// virtual render
	// virtual restoreRenderState
	class TestRendererSystem : public ecs::sys::System<TestRendererSystem>
	{
	public:
		using this_t = TestRendererSystem;
		using base_t = ecs::sys::System<TestRendererSystem>;


	public:
		TestRendererSystem(ecs::sys::SystemManager* manager);


	public:
		virtual void update(ecs::Time t, ecs::Time dt) override;


	private:
		gl::Shader m_vert{};
		gl::Shader m_frag{};
		gl::ShaderProgram m_program{};
		GLint m_pvmLocation{};
		GLint m_mLocation{};
		GLint m_eyeLocation{};
	};
}