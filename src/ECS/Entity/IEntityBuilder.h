#pragma once

#include "Entity.h"
#include "../Utility/NoCopyMove.h"
#include "../Utility/StaticType.h"


namespace ecs::entity
{
	using ecs::Id;
	using ecs::BAD_ID;

	using ECRegistry = EntityComponentRegistry;


	class EntityBuilderManager;

	class IEntityBuilder : public ecs::util::NoCopyMove, public ecs::util::StaticType
	{
	public:
		IEntityBuilder(EntityBuilderManager* manager, Id staticTypeId);

		virtual ~IEntityBuilder() = default;


	public:
		virtual Entity buildEntity() = 0;


	public:
		EntityBuilderManager* getManagerInstance() const;


	private:
		EntityBuilderManager* m_manager{nullptr};
	};
}