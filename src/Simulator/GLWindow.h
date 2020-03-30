#pragma once

#include "../Context/Context.h"

namespace sim
{
	using context::Hint;
	using context::Hints;
	using context::CreationInfo;
	using context::BaseWindow;
	using context::IMediator;


	// owned by Simulator
	class GLWindow : public BaseWindow
	{		
	public:
		GLWindow(IMediator* inputRedir, const CreationInfo& info = {}, bool initGL = true);

		GLWindow(const GLWindow&) = delete;
		GLWindow(GLWindow&&);

		virtual ~GLWindow() = default;

		GLWindow& operator = (const GLWindow&) = delete;
		GLWindow& operator = (GLWindow&&);


	public:
		IMediator* getMediator() const;

		void setMediator(IMediator* inputRedir);


	public:
		virtual void mouseEvent(double xPos, double yPos) override;

		virtual void mouseButtonEvent(int button, int action, int mods) override;

		virtual void scrollEvent(double xOffset, double yOffset) override;

		virtual void keyEvent(int key, int scancode, int action, int mods) override;


	private:
		IMediator* m_inputRedir{nullptr};
		bool m_glInitialized{false};
	};
}