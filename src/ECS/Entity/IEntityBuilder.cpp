#include "IEntityBuilder.h"


namespace ecs::entity
{
	IEntityBuilder::IEntityBuilder(EntityBuilderManager* manager, Id staticTypeId)
		: m_manager(manager)
		, ecs::util::StaticType(staticTypeId)
	{}

	EntityBuilderManager* IEntityBuilder::getManagerInstance() const
	{
		return m_manager;
	}
}
