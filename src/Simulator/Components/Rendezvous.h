#pragma once

#include <Math/MathLib.h>

#include <ECS/ecs_common.h>
#include <ECS/Entity/Entity.h>
#include <ECS/Utility/Counter.h>
#include <ECS/Utility/StaticType.h>

#include "Timer.h"

namespace comp
{
	using ecs::Id;
	using ecs::entity::Entity;

	using ecs::BAD_ID;
	using ecs::entity::null;

	using math::Float;
	using math::Vec3;
	using math::UInt64;


	namespace detail
	{
		template<class T>
		struct BaseAction : ecs::util::StaticType
		{
			static const Id TYPE_ID;

			BaseAction() : ecs::util::StaticType(TYPE_ID)
			{}
		};

		template<class T>
		const Id BaseAction<T>::TYPE_ID = ecs::util::TypeCounter<Id, T, BaseAction>::get(); 
	}


	struct Action
	{
		Entity nextAction{null};

		Id actionStaticType{BAD_ID};
	};

	struct Impuls : detail::BaseAction<Impuls>
	{
		Impuls(const Vec3& dvInit) : dv(dvInit)
		{}

		Vec3 dv{};
	};

	struct Wait : detail::BaseAction<Wait>
	{
		Wait(const Tick& tInit) : duration(tInit)
		{}

		Tick duration{};
	};

	struct Rendezvous
	{
		Float propellantUsed{};

		Entity actionHead{null};
		Entity actionTail{null};
	};
}