#include "TestRendererSystem.h"

#include <ECS/ecs_engine.h>
#include <ECS/System/SystemManager.h>

#include <Math/MathLib.h>
#include <GraphicsEngine/ResourceLoaders/ShaderLoaders/ShaderLoader.h>
#include <GraphicsEngine/ResourceLoaders/ShaderProgramBuilders/ShaderProgramBuilder.h>

#include "../Components/TestRendererTag.h"
#include "../Components/Transform.h"
#include "../Components/Camera.h"

#include "MeshSystem.h"
#include "ContextSystem.h"
#include "GraphicsSystem.h"
#include "SimulatorState.h"

#include "../Test.h"


namespace sim
{
	using math::Mat4f32;

	TestRendererSystem::TestRendererSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		graphics::SimpleShaderLoader loader;
		graphics::ShaderProgramBuilder builder;

		m_vert = loader.shaderFromFile(gl::ShaderType::Vertex, "assets/shaders/test.vert");
		m_frag = loader.shaderFromFile(gl::ShaderType::Fragment, "assets/shaders/test.frag");
		m_program = builder.buildProgram("test program", m_vert, m_frag);
		m_pvmLocation = m_program.getUniformLocation("uPVM");
		m_mLocation  = m_program.getUniformLocation("uM");
		m_eyeLocation = m_program.getUniformLocation("uEye");
	}

	void TestRendererSystem::update(ecs::Time t, ecs::Time dt)
	{
		auto* systemManager  = getSystemManager();
		auto* simulatorState = systemManager->get<SimulatorState>();
		auto& registry       = systemManager->getECSEngine()->getRegistry();

		auto contextSystem = systemManager->get<ContextSystem>();
		if (!contextSystem)
		{
			// TODO : notify about error
			return;
		}

		auto meshSystem = systemManager->get<MeshSystem>();
		if (!meshSystem)
		{
			// TODO : notify about error
			return;
		}

		auto sphereResource = meshSystem->acquireResource(MeshSystem::CUBE).lock();
		if (!sphereResource)
		{
			// TODO : notify about error
			return;
		}

		auto player = simulatorState->getPlayer();
		if (!registry.valid(player) || !registry.has<comp::Camera3rdPerson>(player))
		{
			// TODO : notify about error
			return;
		}
		auto& camera = registry.get<comp::Camera3rdPerson>(player);

		auto usesIndices  = sphereResource->usingIndices();
		auto& drawInfo    = sphereResource->getDrawInfo();
		auto& vertexArray = sphereResource->getVertexArray();

		// set render state
		gl::state::enable(gl::Capability::DepthTest);
		gl::state::disable(gl::Capability::CullFace);

		gl::Framebuffer frame = gl::Framebuffer::getDefaultFramebuffer();
		frame.clearColor(0.0, 0.0, 0.0, 0.0);
		frame.clearDepth(1.0);
		frame.clear(gl::ClearMask::ColorDepth);

		m_program.bind();

		vertexArray.bind();

		// render
		auto p = contextSystem->getProjection();
		auto v = camera.viewMat();
		auto pv = p * v;

		for (auto e : registry.view<comp::TestRendererTag, comp::Transform>())
		{
			auto& transform = registry.get<comp::Transform>(e);

			auto m = transform.toMat();
			m_program.setUniformMat4f32(m_pvmLocation, pv * m);
			m_program.setUniformMat4f32(m_mLocation, m);
			m_program.setUniformVec3f32(m_eyeLocation, camera.getPosition());
			if (!usesIndices)
			{
				vertexArray.drawArrays(drawInfo);
			}
			else
			{
				vertexArray.drawElements(drawInfo);
			}
		}

		// restore render state
		vertexArray.unbind();

		m_program.unbind();
	}
}