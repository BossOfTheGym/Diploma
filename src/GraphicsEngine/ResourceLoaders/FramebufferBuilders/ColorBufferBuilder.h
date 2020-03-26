#pragma once

#include <glWrap/Framebuffer/Framebuffer.h>
#include <glWrap/Texture/Texture.h>

#include <Utility/PureType.h>

namespace graphics
{
	template<class ColorBuilder>
	class ColorBufferBuilder
	{
	public:
		using ColorBuilderType = util::remove_cv_ref_t<ColorBuilder>;

		struct FramebufferResources
		{
			gl::Framebuffer frame;
			gl::Texture     color;
		};


	public:
		template<class CB>
		ColorBufferBuilder(CB&& colorBuilderInit)
			: m_colorBuilder(std::forward<CB>(colorBuilderInit))
		{}

		ColorBufferBuilder(const ColorBufferBuilder&) = default;
		ColorBufferBuilder(ColorBufferBuilder&&)      = default;

		~ColorBufferBuilder() = default;

		ColorBufferBuilder& operator = (const ColorBufferBuilder&) = default;
		ColorBufferBuilder& operator = (ColorBufferBuilder&&)      = default;


	public:
		FramebufferResources buildFramebuffer()
		{
			FramebufferResources result;

			result.color = m_colorBuilder.buildTexture();

			result.frame.createFramebuffer();
			result.frame.namedFramebufferTexture(
				static_cast<gl::FramebufferAttachment>(gl::FramebufferAttachments::Color)
				, result.color
				, 0
			);

			auto status = result.frame.checkNamedFramebufferStatus(gl::FramebufferTarget::Framebuffer);
			if (status == gl::FramebufferStatus::Complete)
			{
				std::cout << "Successfully created color framebuffer" << std::endl;
			}
			else
			{
				std::cout << "Failed to create color framebuffer" << std::endl;
			}

			gl::DrawBuffer buffer = static_cast<gl::DrawBuffer>(gl::FramebufferDrawBuffer::ColorAttachment0);
			result.frame.namedDrawBuffers(1, &buffer);

			return result;
		}


	private:
		ColorBuilderType m_colorBuilder;
	};

	template<class CB>
	ColorBufferBuilder(CB&&) -> ColorBufferBuilder<util::remove_cv_ref_t<CB>>;
}
