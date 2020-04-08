#pragma once

#include <ECS/Entity/EntityBuilder.h>


namespace ecs::entity
{
	class EnityBuilderManager;
}

namespace sim
{
	using ecs::entity::Entity;
	using ecs::entity::null;
	using ECRegistry = ecs::entity::EntityComponentRegistry;
}