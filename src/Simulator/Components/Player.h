#pragma once

#include <ECS/Entity/Entity.h>

namespace comp
{
	using ecs::entity::Entity;
	using ecs::entity::null;

	struct Player
	{
		Entity view{null};
	};
}
