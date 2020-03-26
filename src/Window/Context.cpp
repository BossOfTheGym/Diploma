#include "Context.h"


namespace context
{
	static bool gs_contextInitialized = false;

	bool initialize()
	{
		auto glfwErr = glfwInit();

		gs_contextInitialized = (glfwErr == GLFW_TRUE);

		return initialized;
	}

	bool intiailized()
	{
		return initialized;
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


	void BaseWindow::addWindowToRegister(BaseWindow* window)
	{
		s_windowRegister.insert({window->m_window, window});
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
	BaseWindow::BaseWindow(const CreationInfo& info)
		: m_info(info)
		, m_window()
	{
		for (auto&[hint, value] : info.hints)
		{
			glfwWindowHint(hint, value);
		}

		m_window = glfwCreateWindow(info.width, info.height, info.name.c_str(), nullptr, nullptr);		

		addWindowToRegister(this);
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
	{
		// do nothing
	}

	void BaseWindow::mouseButtonEvent(int button, int action, int mods)
	{
		// do nothing
	}

	void BaseWindow::scrollEvent(double xOffset, double yOffset)
	{
		// do nothing
	}

	void BaseWindow::keyEvent(int key, int scancode, int action, int mods)
	{
		// do nothing
	}
}
