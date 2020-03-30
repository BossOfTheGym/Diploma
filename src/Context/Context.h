#pragma once

#include <Common.h>

#include <Math/MathLib.h>
#include <glWrap/OpenGL.h>
#include <GLFW/glfw3.h>


namespace context
{
	using math::Vec2f64;
	using math::Vec3i;

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

	// TODO : add enums associated with all GLFW constants 

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


		static void addWindowToRegister(BaseWindow& window);

		static Optional getWindowFromRegister(GLFWwindow* window);

		static bool hasRegisterWindow(GLFWwindow* window);

		static void removeWindowFromRegister(GLFWwindow* window);


		static void setAllCommonCallbacks(BaseWindow& window);


	public:
		BaseWindow(const CreationInfo& info, bool initializeGL = true);

		BaseWindow(const BaseWindow&) = delete;
		BaseWindow(BaseWindow&&);

		virtual ~BaseWindow();

		BaseWindow& operator = (const BaseWindow&) = delete;
		BaseWindow& operator = (BaseWindow&&);


	public:
		void initGL();

		bool glInitialized() const;


		void showWindow();

		void hideWindow();

		void makeContextCurrent();

		void swapBuffers();

		void setWindowTitle(const String& title);


		// TODO : add input functions
		Vec2f64 getCursorPos();

		void setCursorPos(const Vec2f64& pos);

		Vec3i getWindowSize();


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
		CreationInfo m_info{};
		GLFWwindow*  m_window{nullptr};
		bool         m_glInitialized{false};
	};

	class IMediator
	{
	public:
		virtual void mouseEvent(BaseWindow& window, double xPos, double yPos) = 0;

		virtual void mouseButtonEvent(BaseWindow& window, int button, int action, int mods) = 0;

		virtual void scrollEvent(BaseWindow& window, double xOffset, double yOffset) = 0;

		virtual void keyEvent(BaseWindow& window, int key, int scancode, int action, int mods) = 0;
	};
}