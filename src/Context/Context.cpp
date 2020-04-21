#include "Context.h"


namespace context
{
	static bool gs_contextInitialized = false;

	bool initialize()
	{
		auto glfwErr = glfwInit();

		gs_contextInitialized = (glfwErr == GLFW_TRUE);

		return gs_contextInitialized;
	}

	bool initialized()
	{
		return gs_contextInitialized;
	}

	void terminate()
	{
		if (gs_contextInitialized)
		{
			glfwTerminate();
			gs_contextInitialized = false;
		}
	}


	void setSwapInterval(int interval)
	{
		glfwSwapInterval(interval);
	}

	void pollEvents()
	{
		glfwPollEvents();
	}


	// statics
	using Register = BaseWindow::Register;
	Register BaseWindow::sm_windowRegister;

	using Optional = BaseWindow::Optional;


	void BaseWindow::commonCursorPosCallback(GLFWwindow* window, double xPos, double yPos)
	{
		if (auto opt = getWindowFromRegister(window); opt)
		{
			BaseWindow& wind = **opt;

			wind.mouseEvent(xPos, yPos);
		}
	}

	void BaseWindow::commonMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (auto opt = getWindowFromRegister(window); opt)
		{
			BaseWindow& wind = **opt;
			
			wind.mouseButtonEvent(button, action, mods);
		}
	}

	void BaseWindow::commonScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		if (auto opt = getWindowFromRegister(window); opt)
		{
			BaseWindow& wind = **opt;
			
			wind.scrollEvent(xOffset, yOffset);
		}
	}

	void BaseWindow::commonKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (auto opt = getWindowFromRegister(window); opt)
		{
			BaseWindow& wind = **opt;
			
			wind.keyEvent(key, scancode, action, mods);
		}
	}


	void BaseWindow::addWindowToRegister(BaseWindow& window)
	{
		sm_windowRegister.insert({window.m_window, &window});
	}

	Optional BaseWindow::getWindowFromRegister(GLFWwindow* window)
	{
		auto it = sm_windowRegister.find(window);
		if (it != sm_windowRegister.end())
		{
			return it->second;
		}
		return std::nullopt;
	}

	bool BaseWindow::hasRegisterWindow(GLFWwindow* window)
	{
		return sm_windowRegister.find(window) != sm_windowRegister.end();
	}

	void BaseWindow::removeWindowFromRegister(GLFWwindow* window)
	{
		sm_windowRegister.erase(window);
	}


	void BaseWindow::setAllCommonCallbacks(BaseWindow& window)
	{
		auto handle = window.getHandle();

		glfwSetKeyCallback(handle, commonKeyCallback);
		glfwSetCursorPosCallback(handle, commonCursorPosCallback);
		glfwSetScrollCallback(handle, commonScrollCallback);
		glfwSetMouseButtonCallback(handle, commonMouseButtonCallback);
	}


	// non-static
	// TODO : little bit messy, STARTED EXPERIMENTING
	// TODO : add ability to create window in full-windowed mode
	BaseWindow::BaseWindow(const CreationInfo& info, bool initializeGL)
		: m_info(info)
	{
		if (sm_windowRegister.empty() && !initialized())
		{
			context::initialize();
		}

		for (auto&[hint, value] : info.hints)
		{
			glfwWindowHint(hint, value);
		}

		//GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		//const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
		//m_window = glfwCreateWindow(videoMode->width, videoMode->height, info.name.c_str(), primaryMonitor, nullptr);		
		m_window = glfwCreateWindow(info.width, info.height, info.name.c_str(), nullptr, nullptr);
		if (m_window)
		{
			addWindowToRegister(*this);
			setAllCommonCallbacks(*this);
			if (initializeGL)
			{
				initGL();
			}
		}
	}

	BaseWindow::~BaseWindow()
	{
		removeWindowFromRegister(m_window);
		if (m_window)
		{
			glfwDestroyWindow(m_window);
		}

		if (sm_windowRegister.empty() && initialized())
		{
			context::terminate();
		}
	}


	void BaseWindow::initGL()
	{
		if (valid() && !m_glInitialized)
		{
			makeContextCurrent();

			m_glInitialized = gl::initialize();
		}
	}

	bool BaseWindow::glInitialized() const
	{
		return m_glInitialized;
	}


	void BaseWindow::showWindow()
	{
		glfwShowWindow(m_window);
	}

	void BaseWindow::hideWindow()
	{
		glfwHideWindow(m_window);
	}

	void BaseWindow::makeContextCurrent()
	{
		glfwMakeContextCurrent(m_window);
	}

	void BaseWindow::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	void BaseWindow::setWindowTitle(const String& title)
	{
		glfwSetWindowTitle(m_window, title.c_str());
	}

	void BaseWindow::disableCursor()
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void BaseWindow::enableCursor()
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	Vec2f64 BaseWindow::getCursorPos()
	{
		Vec2f64 pos;
		glfwGetCursorPos(m_window, &pos.x, &pos.y);
		return pos;
	}

	void BaseWindow::setCursorPos(const Vec2f64& pos)
	{
		glfwSetCursorPos(m_window, pos.x, pos.y);
	}

	Vec2i BaseWindow::getWindowSize()
	{
		Vec2i size;
		glfwGetWindowSize(m_window, &size.x, &size.y);
		return size;
	}


	bool BaseWindow::shouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}

	bool BaseWindow::valid() const
	{
		return m_window != nullptr;
	}

	GLFWwindow* BaseWindow::getHandle() const
	{
		return m_window;
	}


	// virtuals
	void BaseWindow::mouseEvent(double xPos, double yPos)
	{}

	void BaseWindow::mouseButtonEvent(int button, int action, int mods)
	{}

	void BaseWindow::scrollEvent(double xOffset, double yOffset)
	{}

	void BaseWindow::keyEvent(int key, int scancode, int action, int mods)
	{}
}
