#pragma once

#include <Common.h>

#include <GLFW/glfw3.h>


namespace context
{
	bool initialize();

	bool initialized();

	void terminate();


	void pollEvents();


	using alias::String;

	struct Hint
	{
		int hint  = 0;
		int value = 0;
	};

	using Hints = std::vector<Hint>;

	struct CreationInfo
	{
		int width{};
		int height{};

		String name{};

		Hints hints{};
	};

	
	class BaseWindow
	{
	public:
		using Register = std::unordered_map<GLFWwindow*, BaseWindow*>;

		using Optional = std::optional<BaseWindow*>;


	private:
		static Register s_windowRegister;


		static void commonCursorPosCallback(GLFWwindow* window, double xPos, double yPos);

		static void commonMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		static void commonScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

		static void commonKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		/* TODO
		static void commonCharCallback();

		static void commonResizeCallback();

		static void commonCursorPosCallback();

		static void commonCursorEnterCallback();

		static void commonDropCallback();

		static void commonCloseCallback();

		static void commonFramebufferSizeCallback();

		static void commonWindowPosCallback();
		*/


		static void addWindowToRegister(BaseWindow* window);

		static Optional getWindowFromRegister(GLFWwindow* window);

		static bool hasRegisterWindow(GLFWwindow* window);

		static void removeWindowFromRegister(GLFWwindow* window);


		static void setAllCommonCallbacks(BaseWindow& window);


	public:
		BaseWindow(const CreationInfo&);

		BaseWindow(const BaseWindow&) = delete;
		BaseWindow(BaseWindow&&);

		virtual ~BaseWindow();

		BaseWindow& operator = (const BaseWindow&) = delete;
		BaseWindow& operator = (BaseWindow&&);


	public:
		void showWindow();

		void hideWindow();

		void makeContextCurrent();

		void swapBuffers();

		void setWindowTitle(const String& title);

		bool shouldClose();

		bool valid() const;


		GLFWwindow* getHandle() const;


	public:
		virtual void mouseEvent(double xPos, double yPos);

		virtual void mouseButtonEvent(int button, int action, int mods);

		virtual void scrollEvent(double xOffset, double yOffset);

		virtual void keyEvent(int key, int scancode, int action, int mods);


		/* TODO
		virtual void charEvent();

		virtual void resizeEvent();

		virtual void cursorPosEvent();

		virtual void cursorEnterEvent();

		virtual void dropEvent();

		virtual void closeEvent();

		virtual void framebufferSizeEvent();

		virtual void windowPosEvent();
		*/


	protected:
		GLFWwindow* m_window{};
		CreationInfo m_info{};
	};
}