#pragma once

#include <entt/entt.hpp>

#include "GLWindow.h"
#include "../GraphicsEngine/GraphicsEngine.h"

namespace sim
{
	class Simulator
	{
	public:
		Simulator();

		Simulator(const Simulator&) = delete;
		Simulator(Simulator&&);

		~Simulator();

		Simulator& operator = (const Simulator&) = delete;
		Simulator& operator = (Simulator&&);


	public:
		void mainLoop();
		

	private:
		entt::registry           m_registry;
		graphics::GraphicsEngine m_grahicsEngine;

		GLWindow                 m_window;
	};
}
