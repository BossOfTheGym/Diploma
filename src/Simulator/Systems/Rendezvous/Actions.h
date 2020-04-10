#pragma once

#include <ECS/ecs_common.h>
#include <ECS/Utility/StaticType.h>

#include "../../Components/Rendezvous.h"

namespace sim
{
	using ecs::Id;
	using ecs::entity::Entity;

	using ecs::BAD_ID;
	using ecs::entity::null;

	
	namespace detail
	{
		class IAction : public ecs::util::StaticType
		{
		public:
			virtual void update(Entity e) = 0;
		};

		template<class T>
		class BaseAction : public IAction
		{
		public:
			static const Id TYPE_ID;

		public:
			virtual void update(Entity e) override
			{}
		};

		template<class T>
		const Id BaseAction<T>::TYPE_ID = T::TYPE_ID;
	}

	// TODO
	class Impuls : public detail::BaseAction<comp::Impuls>
	{
	public:
		
	};

	// TODO
	class Wait : public detail::BaseAction<comp::Wait>
	{
	public:

	};
}