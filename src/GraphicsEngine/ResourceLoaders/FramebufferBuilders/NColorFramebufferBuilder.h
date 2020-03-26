#pragma once

#include <glWrap/Framebuffer/Framebuffer.h>
#include <glWrap/Texture/Texture.h>
#include <Utility/PureType.h>

#include <array>

namespace graphics
{
	template<size_t V>
	struct value
	{
		static constexpr size_t val = V;
	};

	template<class ColorBuilder, class DepthBuilder, size_t Colors = 1>
	class NColorFBBuilder
	{
	public:
		using ColorBuilderType = util::remove_cv_ref_t<ColorBuilder>;
		using DepthBuilderType = util::remove_cv_ref_t<DepthBuilder>;

		struct FramebufferResources
		{
			gl::Framebuffer frame;
			std::array<gl::Texture, Colors> colors;
			gl::Texture depth;
		};
		

	public:
		template<class CB, class DB, size_t ColorsCount>
		NColorFBBuilder(CB&& colorBuilder, DB&& depthBuilder, value<ColorsCount>)
			: m_colorBuilder(std::forward<CB>(colorBuilder))
			, m_depthBuilder(std::forward<DB>(depthBuilder))
		{}

	public:
		FramebufferResources buildFramebuffer()
		{
			FramebufferResources res;

			for (size_t i = 0; i < Colors; i++)
			{
				res.colors[i] = m_colorBuilder.buildTexture();
			}
			res.depth = m_depthBuilder.buildTexture();

			auto colorAttachment = static_cast<gl::FramebufferAttachment>(gl::FramebufferAttachments::Color);
			auto depthAttachment = static_cast<gl::FramebufferAttachment>(gl::FramebufferAttachments::Depth);
			res.frame.createFramebuffer();
			for (size_t i = 0; i < Colors; i++)
			{
				res.frame.namedFramebufferTexture(colorAttachment + i, res.colors[i], 0);
			}
			res.frame.namedFramebufferTexture(depthAttachment, res.depth, 0);

			auto status = res.frame.checkNamedFramebufferStatus(gl::FramebufferTarget::Framebuffer);
			if (status == gl::FramebufferStatus::Complete)
			{
				std::cout << "Successfully created n-color framebuffer" << std::endl;
			}
			else
			{
				std::cout << "Failed to create n-color framebuffer" << std::endl;
			}

			auto drawBuffer = static_cast<gl::DrawBuffer>(gl::FramebufferDrawBuffer::ColorAttachment0);
			std::array<gl::DrawBuffer, Colors> buffers;
			for(size_t i = 0; i < Colors; i++)
			{
				buffers[i] = drawBuffer + i;
			}
			res.frame.namedDrawBuffers(Colors, &buffers[0]);

			return res;
		}

	private:
		ColorBuilderType m_colorBuilder;
		DepthBuilderType m_depthBuilder;
	};

	template<class CB, class DB, size_t ColorsCount>
	NColorFBBuilder(CB&&, DB&&, value<ColorsCount>) 
		-> NColorFBBuilder<util::remove_cv_ref_t<CB>, util::remove_cv_ref_t<DB>, ColorsCount>;
}