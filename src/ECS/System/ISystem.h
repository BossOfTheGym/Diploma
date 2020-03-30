#pragma once

#include "../Config/Config.h"

namespace ecs
{
	using ecs::config::Id;
	using ecs::config::Float;

	class ISystem
	{
	public:
		virtual void update(Float& t, Float& dt) = 0;
		
		virtual Id getStaticTypeId() = 0;


	public:
		void disable();

		void enable();

		bool enabled();


	private:
		bool m_enabled;
	};
}