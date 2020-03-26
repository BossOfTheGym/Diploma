#pragma once

#include <glWrap/Framebuffer/Framebuffer.h>
#include <glWrap/Texture/Texture.h>

#include <Utility/PureType.h>

namespace graphics
{
	template<class ColorBuilder, class DepthBuilder>
	class SimpleFramebufferBuilder
	{
	public:
		using ColorBuilderType = util::remove_cv_ref_t<ColorBuilder>;
		using DepthBuilderType = util::remove_cv_ref_t<DepthBuilder>;

		struct FramebufferResources
		{
			gl::Framebuffer frame;
			gl::Texture     color;
			gl::Texture     depth;
		};

	public:
		template<class CB, class DB>
		SimpleFramebufferBuilder(CB&& colorBuilderInit,DB&& depthBuilderInit)
			: m_colorBuilder(std::forward<CB>(colorBuilderInit))
			, m_depthBuilder(std::forward<DB>(depthBuilderInit))
		{}

		SimpleFramebufferBuilder(const SimpleFramebufferBuilder&) = default;
		SimpleFramebufferBuilder(SimpleFramebufferBuilder&&)      = default;

		~SimpleFramebufferBuilder() = default;

		SimpleFramebufferBuilder& operator = (const SimpleFramebufferBuilder&) = default;
		SimpleFramebufferBuilder& operator = (SimpleFramebufferBuilder&&)      = default;


	public:
		FramebufferResources buildFramebuffer()
		{
			FramebufferResources result;

			result.color = m_colorBuilder.buildTexture();
			result.depth = m_depthBuilder.buildTexture();

			result.frame.createFramebuffer();
			result.frame.namedFramebufferTexture(
				static_cast<gl::FramebufferAttachment>(gl::FramebufferAttachments::Color)
				, result.color
				, 0
			);
			result.frame.namedFramebufferTexture(
				static_cast<gl::FramebufferAttachment>(gl::FramebufferAttachments::Depth)
				, result.depth
				, 0
			);

			auto status = result.frame.checkNamedFramebufferStatus(gl::FramebufferTarget::Framebuffer);
			if (status == gl::FramebufferStatus::Complete)
			{
				std::cout << "Successfully created framebuffer" << std::endl;
			}
			else
			{
				std::cout << "Failed to create framebuffer" << std::endl;
			}

			gl::DrawBuffer buffer = static_cast<gl::DrawBuffer>(gl::FramebufferDrawBuffer::ColorAttachment0);
			result.frame.namedDrawBuffers(1, &buffer);

			return result;
		}


	private:
		ColorBuilderType m_colorBuilder;
		DepthBuilderType m_depthBuilder;
	};

	template<class CB, class DB>
	SimpleFramebufferBuilder(CB&&, DB&&) 
		-> SimpleFramebufferBuilder<util::remove_cv_ref_t<CB>, util::remove_cv_ref_t<DB>>;
}
