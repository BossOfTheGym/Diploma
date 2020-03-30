#include "GLWindow.h"
#include "GLWindow.h"
#include "GLWindow.h"
#include "GLWindow.h"
#include "GLWindow.h"
#include "GLWindow.h"
#include "GLWindow.h"

#include <utility>

#include <glWrap/OpenGL.h>


namespace sim
{
	GLWindow::GLWindow(IMediator* inputRedir, const CreationInfo& info, bool initializeGL)
		: BaseWindow(info, initializeGL)
		, m_inputRedir(inputRedir)
	{}

	GLWindow::GLWindow(GLWindow&& another) : BaseWindow(std::move(another))
	{
		*this = std::move(another);
	}

	GLWindow& GLWindow::operator = (GLWindow&& another)
	{
		std::swap(m_inputRedir, another.m_inputRedir);
		std::swap(m_glInitialized, another.m_glInitialized);

		return *this;
	}


	IMediator* GLWindow::getMediator() const
	{
		return m_inputRedir;
	}

	void GLWindow::setMediator(IMediator* inputRedir)
	{
		m_inputRedir = inputRedir;
	}


	void GLWindow::mouseEvent(double xPos, double yPos)
	{
		if (m_inputRedir)
		{
			m_inputRedir->mouseEvent(*this, xPos, yPos);
		}
	}

	void GLWindow::mouseButtonEvent(int button, int action, int mods)
	{
		if (m_inputRedir)
		{
			m_inputRedir->mouseButtonEvent(*this, button, action, mods);
		}
	}

	void GLWindow::scrollEvent(double xOffset, double yOffset)
	{
		if (m_inputRedir)
		{
			m_inputRedir->scrollEvent(*this, xOffset, yOffset);
		}
	}

	void GLWindow::keyEvent(int key, int scancode, int action, int mods)
	{
		if (m_inputRedir)
		{
			m_inputRedir->keyEvent(*this, key, scancode, action, mods);
		}
	}
}