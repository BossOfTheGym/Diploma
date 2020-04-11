#include "TestRendererSystem.h"

#include <Math/MathLib.h>
#include <GraphicsEngine/ResourceLoaders/ShaderLoaders/ShaderLoader.h>
#include <GraphicsEngine/ResourceLoaders/ShaderProgramBuilders/ShaderProgramBuilder.h>

#include "../Components/TestRendererTag.h"
#include "../Components/Transform.h"

#include "MeshSystem.h"
#include "ContextSystem.h"
#include "GraphicsSystem.h"

#include "../Simulator.h"
#include "../Test.h"


namespace sim
{
	using math::Mat4f32;

	TestRendererSystem::TestRendererSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		// TODO : add assert
		m_simulator = static_cast<Simulator*>(manager->getECSEngine());

		graphics::SimpleShaderLoader loader;
		graphics::ShaderProgramBuilder builder;

		m_vert = loader.shaderFromFile(gl::ShaderType::Vertex, "assets/shaders/test.vert");
		m_frag = loader.shaderFromFile(gl::ShaderType::Fragment, "assets/shaders/test.frag");
		m_program = builder.buildProgram("test program", m_vert, m_frag);
		m_pvmLocation = m_program.getUniformLocation("uPVM");
	}

	void TestRendererSystem::update(ecs::Float t, ecs::Float dt)
	{
		auto& registry      = m_simulator->getRegistry();
		auto& systemManager = m_simulator->getSystemManager();

		auto contextSystem = systemManager.get<ContextSystem>();
		if (!contextSystem)
		{
			// TODO : notify about error
			return;
		}

		auto meshSystem = systemManager.get<MeshSystem>();
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

		auto usesIndices  = sphereResource->usingIndices();
		auto& drawInfo    = sphereResource->getDrawInfo();
		auto& vertexArray = sphereResource->getVertexArray();

		// to set render state
		gl::state::enable(gl::Capability::DepthTest);
		gl::state::disable(gl::Capability::CullFace);

		gl::Framebuffer frame = gl::Framebuffer::getDefaultFramebuffer();
		frame.clearColor(0.0, 0.0, 0.0, 0.0);
		frame.clearDepth(1.0);
		frame.clear(gl::ClearMask::ColorDepth);

		m_program.bind();

		vertexArray.bind();

		// render
		auto pv = contextSystem->getProjection() * test::TEST_VIEW;
		auto view = registry.view<comp::TestRendererTag, comp::Transform>();
		for (auto e : view)
		{
			auto& transform = registry.get<comp::Transform>(e);

			m_program.setUniformMat4f32(m_pvmLocation, pv * transform.toMat());
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