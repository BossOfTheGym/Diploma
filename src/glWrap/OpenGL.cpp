#include "OpenGL.h"

namespace gl::state
{
	bool initializeLoader()
	{
		auto glewErr = glewInit();

		return glewErr == GLEW_OK;
	}

	//state management
	void blendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		glBlendColor(red, green, blue, alpha);
	}
	
	void blendEquation(BlendMode mode)
	{
		glBlendEquation(static_cast<GLenum>(mode));
	}

	void blendEquationi(BufferIndex number, BlendMode mode)
	{
		glBlendEquationi(number, static_cast<GLenum>(mode));
	}

	void blendEquationSeparate(BlendMode rgb, BlendMode alpha)
	{
		glBlendEquationSeparate(static_cast<GLenum>(rgb), static_cast<GLenum>(alpha));
	}

	void blendEquationSeparatei(BufferIndex number, BlendMode rgb, BlendMode alpha)
	{
		glBlendEquationSeparatei(number, static_cast<GLenum>(rgb), static_cast<GLenum>(alpha));
	}

	void blendFunc(BlendFactor srcFactor, BlendFactor dstFactor)
	{
		glBlendFunc(static_cast<GLenum>(srcFactor), static_cast<GLenum>(dstFactor));
	}

	void blendFunci(BufferIndex number, BlendFactor srcFactor, BlendFactor dstFactor)
	{
		glBlendFunci(number, static_cast<GLenum>(srcFactor), static_cast<GLenum>(dstFactor));
	}

	void blendFuncSeparate(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha)
	{
		glBlendFuncSeparate(
			static_cast<GLenum>(srcRGB)
			, static_cast<GLenum>(dstRGB)
			, static_cast<GLenum>(srcAlpha)
			, static_cast<GLenum>(dstAlpha)
		);
	}

	void blendFuncSeparatei(BufferIndex number, BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha)
	{
		glBlendFuncSeparatei(
			number
			, static_cast<GLenum>(srcRGB)
			, static_cast<GLenum>(dstRGB)
			, static_cast<GLenum>(srcAlpha)
			, static_cast<GLenum>(dstAlpha)
		);
	}


	void cullFace(Face mode)
	{
		glCullFace(static_cast<GLenum>(mode));
	}


	void depthFunc(DepthFunc func)
	{
		glDepthFunc(static_cast<GLenum>(func));
	}


	void enable(Capability parameter)
	{
		glEnable(static_cast<GLenum>(parameter));
	}

	void enablei(Capability parameter, CapabalityIndex index)
	{
		glEnablei(static_cast<GLenum>(parameter), index);
	}

	void disable(Capability parameter)
	{
		glDisable(static_cast<GLenum>(parameter));
	}

	void disablei(Capability parameter, CapabalityIndex index)
	{
		glDisablei(static_cast<GLenum>(parameter), index);
	}


	void frontFace(FrontFace mode)
	{
		glFrontFace(static_cast<GLenum>(mode));
	}


	void polygonMode(Face face, PolygonMode mode)
	{
		glPolygonMode(static_cast<GLenum>(face), static_cast<GLenum>(mode));
	}


	void viewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		glViewport(x, y, width, height);
	}
}