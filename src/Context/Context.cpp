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

	bool intialized()
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


	void pollEvents()
	{
		glfwPollEvents();
	}


	// statics
	using Register = BaseWindow::Register;
	Register BaseWindow::s_windowRegister;

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
		s_windowRegister.insert({window.m_window, &window});
	}

	Optional BaseWindow::getWindowFromRegister(GLFWwindow* window)
	{
		auto it = s_windowRegister.find(window);
		if (it != s_windowRegister.end())
		{
			return it->second;
		}
		return std::nullopt;
	}

	bool BaseWindow::hasRegisterWindow(GLFWwindow* window)
	{
		return s_windowRegister.find(window) != s_windowRegister.end();
	}

	void BaseWindow::removeWindowFromRegister(GLFWwindow* window)
	{
		s_windowRegister.erase(window);
	}


	void BaseWindow::setAllCommonCallbacks(BaseWindow& window)
	{
		auto handle = window.getHandle();

		glfwSetKeyCallback(handle, commonKeyCallback);
		glfwSetCursorPosCallback(handle, commonCursorPosCallback);
		glfwSetScrollCallback(handle, commonScrollCallback);
		glfwSetKeyCallback(handle, commonKeyCallback);
	}


	// non-static
	// TODO : little bit messy, STARTED EXPERIMENTING
	BaseWindow::BaseWindow(const CreationInfo& info, bool initializeGL)
		: m_info(info)
	{
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
			makeContextCurrent();
			glfwSwapInterval(1);
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	BaseWindow::BaseWindow(BaseWindow&& another)
		: m_info()
		, m_window()
	{
		*this = std::move(another);
	}

	BaseWindow::~BaseWindow()
	{
		removeWindowFromRegister(m_window);
		if (m_window)
		{
			glfwDestroyWindow(m_window);
		}
	}

	BaseWindow& BaseWindow::operator = (BaseWindow&& another)
	{
		std::swap(m_window, another.m_window);
		std::swap(m_info, another.m_info);

		return *this;
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

	Vec3i BaseWindow::getWindowSize()
	{
		Vec3i size;
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
