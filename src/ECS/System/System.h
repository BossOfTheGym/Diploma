#pragma once

#include "../Config/Config.h"
#include "../Utility/Family.h"

#include "ISystem.h"

namespace ecs::sys
{
	using ecs::config::Id;
	using ecs::config::Float;
	
	class SystemManager;

	template<class T>
	class System : public ISystem
	{
	public:
		static const Id CLASS_ID;


	public:
		System(SystemManager* manager) : m_manager(manager)
		{}

		System(const System&) = delete;
		System(System&&)      = default;

		virtual ~System() = default;

		System& operator = (const System&) = delete;
		System& operator = (System&&)      = default;


	public:
		virtual void update(Float t, Float dt) override
		{}

		virtual Id getStaticTypeId()
		{
			return CLASS_ID;
		}

	private:
		SystemManager* m_manager{};
	};

	template<class T>
	const Id System<T>::CLASS_ID = ecs::util::Family<ISystem, Id>::id();
}
