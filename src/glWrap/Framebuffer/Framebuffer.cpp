#include "Framebuffer.h"

#include<utility>

#include <glWrap/Texture/Texture.h>

namespace gl
{
	//statics
	Framebuffer Framebuffer::getDefaultFramebuffer()
	{
		//return empty framebuffer(zero id always belongs to default)
		return Framebuffer();
	}


	//constructors & destructor
	Framebuffer::Framebuffer()
	{}

	Framebuffer::Framebuffer(Framebuffer&& buffer) : Id(std::move(buffer))
	{}

	Framebuffer::~Framebuffer()
	{
		deleteFramebuffer();
	}

	//operators
	Framebuffer& Framebuffer::operator = (Framebuffer&& buffer)
	{
		static_cast<Id&>(*this) = static_cast<Id&&>(buffer);

		return *this;
	}

	//core functions
	void Framebuffer::genFramebuffer()
	{
		GLuint framebufferId;

		glGenFramebuffers(1, &framebufferId);

		static_cast<Id&>(*this) = framebufferId;
	}

	void Framebuffer::createFramebuffer()
	{
		GLuint framebufferId;

		glCreateFramebuffers(1, &framebufferId);

		static_cast<Id&>(*this) = framebufferId;
	}


	void Framebuffer::bindFramebuffer(FramebufferTarget target) const
	{
		glBindFramebuffer(static_cast<GLenum>(target), id());
	}

	//doesn't require object's state
	void Framebuffer::unbindFramebuffer(FramebufferTarget target) const
	{
		glBindFramebuffer(static_cast<GLenum>(target), Id::Empty);
	}

	FramebufferStatus Framebuffer::checkNamedFramebufferStatus(FramebufferTarget target) const
	{
		return FramebufferStatus{glCheckNamedFramebufferStatus(id(), static_cast<GLenum>(target))};
	}

	void Framebuffer::blitNamedFramebuffer(
		const Rectangle& destRect
		, const Framebuffer& src
		, const Rectangle& srcRect
		, BlitMask mask
		, FramebufferFilter filter
	)
	{
		auto& [srcX0, srcY0, srcX1, srcY1] = srcRect;
		auto& [destX0, destY0, destX1, destY1] = destRect;

		glBlitNamedFramebuffer(
			src.id(), id()
			, srcX0, srcY0, srcX1, srcY1
			, destX0, destY0, destX1, destY1
			, static_cast<GLbitfield>(mask)
			, static_cast<GLenum>(filter)
		);
	}


	void Framebuffer::namedFramebufferParameteri(FramebufferParameteri name, GLint value)
	{
		glNamedFramebufferParameteri(id(), static_cast<GLenum>(name), value);
	}

	void Framebuffer::namedFramebufferTexture(FramebufferAttachment attachment, const Texture& texture, GLint level)
	{
		glNamedFramebufferTexture(id(), static_cast<GLenum>(attachment), texture.id(), level);
	}

	void Framebuffer::framebufferTextureLayer(FramebufferAttachment attachment, const Texture& texture, GLint level, GLint layer)
	{
		//TODO : add openGl function
	}


	void Framebuffer::namedClearBufferfv(FramebufferClearBufferfv buffer, BufferNumber drawBuffer, GLfloat* value)
	{
		glClearNamedFramebufferfv(id(), static_cast<GLenum>(buffer), drawBuffer, value);
	}

	void Framebuffer::namedClearBufferiv(FramebufferClearBufferiv buffer, BufferNumber drawBuffer, const GLint* value)
	{
		glClearNamedFramebufferiv(id(), static_cast<GLenum>(buffer), drawBuffer, value);
	}

	void Framebuffer::namedClearBufferuiv(FramebufferClearBufferuiv buffer, BufferNumber drawBuffer, const GLuint* value)
	{
		glClearNamedFramebufferuiv(id(), static_cast<GLenum>(buffer), drawBuffer, value);
	}

	void Framebuffer::namedClearBufferfi(FramebufferClearBufferfi buffer, BufferNumber drawBuffer, GLfloat depth, GLint stencil)
	{
		glClearNamedFramebufferfi(id(), static_cast<GLenum>(buffer), drawBuffer, depth, stencil);
	}


	void Framebuffer::clear(ClearMask mask)
	{
		glClear(static_cast<GLbitfield>(mask));
	}

	void Framebuffer::clearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	void Framebuffer::clearDepth(GLfloat depth)
	{
		glClearDepth(depth);
	}

	void Framebuffer::clearStencil(GLint stencil)
	{
		glClearStencil(stencil);
	}


	void Framebuffer::namedDrawBuffer(DrawBuffer buffer)
	{
		glNamedFramebufferDrawBuffer(id(), buffer);
	}

	void Framebuffer::namedDrawBuffers(GLsizei count, const DrawBuffer* bufs)
	{
		glNamedFramebufferDrawBuffers(id(), count, static_cast<const GLenum*>(bufs));
	}


	void Framebuffer::deleteFramebuffer()
	{
		GLuint framebufferID = id();

		glDeleteFramebuffers(1, &framebufferID);
	}

	void Framebuffer::resetFramebuffer()
	{
		resetId();
	}
}