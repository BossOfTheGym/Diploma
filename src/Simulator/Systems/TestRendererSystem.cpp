#include "TestRendererSystem.h"

#include <ECS/ecs_engine.h>
#include <ECS/System/SystemManager.h>

#include <Math/MathLib.h>

#include <GraphicsEngine/ResourceLoaders/ShaderLoaders/ShaderLoader.h>
#include <GraphicsEngine/ResourceLoaders/ShaderProgramBuilders/ShaderProgramBuilder.h>

#include <iostream>

#include "../Components/TestRendererTag.h"
#include "../Components/Transform.h"
#include "../Components/Camera.h"

#include "MeshSystem.h"
#include "ContextSystem.h"
#include "GraphicsSystem.h"
#include "SimulatorState.h"

#include "Rendering/SomeTextureBuilders.h"

#include "../Test.h"


namespace sim
{
	using math::Mat4f32;

	TestRendererSystem::TestRendererSystem(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{
		auto* systemManager  = getSystemManager();
		auto* contextSystem  = systemManager->get<ContextSystem>();

		// shader
		graphics::SimpleShaderLoader loader;
		graphics::ShaderProgramBuilder builder;

		m_vert = loader.shaderFromFile(gl::ShaderType::Vertex, "assets/shaders/test.vert");
		m_frag = loader.shaderFromFile(gl::ShaderType::Fragment, "assets/shaders/test.frag");
		m_program = builder.buildProgram("test program", m_vert, m_frag);
		m_pvmLocation = m_program.getUniformLocation("uPVM");
		m_mLocation  = m_program.getUniformLocation("uM");
		m_eyeLocation = m_program.getUniformLocation("uEye");

		// depth
		auto extents = contextSystem->getWindowSize();
		m_depth.createTexture(gl::TextureTarget::Texture2D);
		m_depth.bind(gl::TextureTarget::Texture2D);
		m_depth.texImage2D(
			gl::Texture::Image2D_Data
			{
				  gl::TextureTarget::Texture2D
				, 0
				, gl::InternalFormat::DepthComponent32F
				, extents.x
				, extents.y
				, gl::PixelDataFormat::DepthComponent
				, gl::DataType::Float
				, nullptr
			}
		);
		m_depth.textureParameteri(gl::TextureParameter::TextureMinFilter, static_cast<GLint>(gl::TextureParameterValue::Nearest)    );
		m_depth.textureParameteri(gl::TextureParameter::TextureMagFilter, static_cast<GLint>(gl::TextureParameterValue::Nearest)    );
		m_depth.textureParameteri(gl::TextureParameter::TextureWrapS    , static_cast<GLint>(gl::TextureParameterValue::ClampToEdge));
		m_depth.textureParameteri(gl::TextureParameter::TextureWrapT    , static_cast<GLint>(gl::TextureParameterValue::ClampToEdge));
		m_depth.unbind(gl::TextureTarget::Texture2D);

		// color
		m_color.createTexture(gl::TextureTarget::Texture2D);
		m_color.bind(gl::TextureTarget::Texture2D);
		m_color.texImage2D(
			gl::Texture::Image2D_Data
			{
				  gl::TextureTarget::Texture2D
				, 0
				, gl::InternalFormat::RGBA32F
				, extents.x
				, extents.y
				, gl::PixelDataFormat::RGBA
				, gl::DataType::Float
				, nullptr
			}
		);
		m_color.textureParameteri(gl::TextureParameter::TextureMinFilter, static_cast<GLint>(gl::TextureParameterValue::Linear)    );
		m_color.textureParameteri(gl::TextureParameter::TextureMagFilter, static_cast<GLint>(gl::TextureParameterValue::Linear)    );
		m_color.textureParameteri(gl::TextureParameter::TextureWrapS    , static_cast<GLint>(gl::TextureParameterValue::ClampToEdge));
		m_color.textureParameteri(gl::TextureParameter::TextureWrapT    , static_cast<GLint>(gl::TextureParameterValue::ClampToEdge));
		m_color.unbind(gl::TextureTarget::Texture2D);

		// framebuffer
		m_offScreen.createFramebuffer();
		m_offScreen.namedFramebufferTexture(
			static_cast<gl::FramebufferAttachment>(gl::FramebufferAttachments::Color)
			, m_color
			, 0
		);
		m_offScreen.namedFramebufferTexture(
			static_cast<gl::FramebufferAttachment>(gl::FramebufferAttachments::Depth)
			, m_depth
			, 0
		);
		auto status = m_offScreen.checkNamedFramebufferStatus(gl::FramebufferTarget::Framebuffer);
		if (status != gl::FramebufferStatus::Complete)
		{
			// DEBUG
			std::cout << "Failed to create color framebuffer" << std::endl;
		}
		auto drawBuffer = static_cast<gl::DrawBuffer>(gl::FramebufferDrawBuffer::ColorAttachment0);
		m_offScreen.namedDrawBuffers(1, &drawBuffer);
	}

	void TestRendererSystem::update(ecs::Time t, ecs::Time dt)
	{
		auto* systemManager  = getSystemManager();
		auto& registry       = systemManager->getECSEngine()->getRegistry();
		auto* simulatorState = systemManager->get<SimulatorState>();
		auto* contextSystem  = systemManager->get<ContextSystem>();

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

		auto sphereResource = meshSystem->acquireResource(MeshSystem::SPHERE).lock();
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

		//// TEST
		//m_offScreen.bindFramebuffer(gl::FramebufferTarget::Framebuffer);
		//m_offScreen.clearColor(0.0, 0.0, 0.0, 1.0);
		//m_offScreen.clearDepth(1.0);
		//m_offScreen.clear(gl::ClearMask::ColorDepth);
		//// END TEST

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

		//// TEST
		//auto extents = contextSystem->getWindowSize();
		//gl::Framebuffer frame = gl::Framebuffer::getDefaultFramebuffer();
		//frame.bindFramebuffer(gl::FramebufferTarget::Framebuffer);
		//frame.clear(gl::ClearMask::Color);
		//frame.blitNamedFramebuffer({0, 0, extents.x, extents.y}, m_offScreen, {0, 0, extents.x, extents.y}, gl::BlitMask::Color, gl::FramebufferFilter::Nearest);
		//// END TEST
	}
}