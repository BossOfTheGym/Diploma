#pragma once

#include "../Utility/Counter.h"

#include "ISystem.h"

namespace ecs::sys
{
	class SystemManager;

	template<class T>
	class System : public ISystem
	{
	public:
		static const Id TYPE_ID;


	public:
		System(SystemManager* manager) : ISystem(manager, TYPE_ID)
		{}


	public:
		virtual void update(Time t, Time dt) override
		{}
	};

	template<class T>
	const Id System<T>::TYPE_ID = ecs::util::TypeCounter<Id, T, ISystem>::get();
}
