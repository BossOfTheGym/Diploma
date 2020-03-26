#pragma once

#include <glWrap/OpenGL.h>
#include <glWrap/ID/Id.h>


namespace gl
{
	class Texture;

	class Framebuffer : public Id
	{
	public:
		struct Rectangle
		{
			GLint x0 = static_cast<GLint>(0);
			GLint y0 = static_cast<GLint>(0);
			GLint x1 = static_cast<GLint>(0);
			GLint y1 = static_cast<GLint>(0);
		};

	public:
		static Framebuffer getDefaultFramebuffer();


	public:
		Framebuffer();

		Framebuffer(Framebuffer&& framebuffer);

		~Framebuffer();

		Framebuffer& operator = (Framebuffer&& framebuffer);


	public:
		//methods depend on object's state
		void genFramebuffer();

		void createFramebuffer();


		void bindFramebuffer(FramebufferTarget target) const;

		void unbindFramebuffer(FramebufferTarget target) const;


		FramebufferStatus checkNamedFramebufferStatus(FramebufferTarget target) const;


		void blitNamedFramebuffer(
			  const Rectangle& destRect
			, const Framebuffer& src
			, const Rectangle& srcRect
			, BlitMask mask
			, FramebufferFilter filter
		);


		void namedFramebufferParameteri(FramebufferParameteri name, GLint value);

		void namedFramebufferTexture(FramebufferAttachment attachment, const Texture& texture, GLint level);


		void framebufferTextureLayer(FramebufferAttachment attachment, const Texture& texture, GLint level, GLint layer);


		void namedClearBufferfv(FramebufferClearBufferfv buffer, BufferNumber drawBuffer, GLfloat* value);

		void namedClearBufferiv(FramebufferClearBufferiv buffer, BufferNumber drawBuffer, const GLint* value);

		void namedClearBufferuiv(FramebufferClearBufferuiv buffer, BufferNumber drawBuffer, const GLuint* value);

		void namedClearBufferfi(FramebufferClearBufferfi buffer, BufferNumber drawBuffer, GLfloat depth, GLint stencil);


		//methods don't depend on object's state
		void clear(ClearMask mask);

		void clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

		void clearDepth(GLfloat depth);

		void clearStencil(GLint stencil);


		//depend
		void namedDrawBuffer(DrawBuffer buffer);

		void namedDrawBuffers(GLsizei count, const DrawBuffer* bufs);


		//depend
		void deleteFramebuffer();

	private:
		void resetFramebuffer();
	};
}