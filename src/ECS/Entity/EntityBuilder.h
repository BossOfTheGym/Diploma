#pragma once

#include "IEntityBuilder.h"
#include "../Utility/Counter.h"

namespace ecs::entity
{
	class EntityBuilderManager;

	template<class T>
	class EntityBuilder : public IEntityBuilder
	{
	public:
		static const Id TYPE_ID;

	public:
		EntityBuilder(EntityBuilderManager* manager)
			: IEntityBuilder(manager, TYPE_ID)
		{}

		virtual ~EntityBuilder() = default;

	public:
		virtual Entity buildEntity() override
		{
			return null;
		}
	};

	template<class T>
	const Id EntityBuilder<T>::TYPE_ID = ecs::util::TypeCounter<Id, T, IEntityBuilder>::get();
}