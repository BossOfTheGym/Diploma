#pragma once

#include "../Config/Config.h"
#include "../Utility/Family.h"

#include "IComponent.h"


namespace ecs::comp
{
	using ecs::config::Id;

	template<class T>
	class Component : public IComponent
	{
	public:
		static const Id CLASS_ID;

	public:
		virtual ~Component() = default;
	};

	template<class T>
	const Id Component<T>::CLASS_ID = ecs::util::Family<IComponent, Id>::id();
}