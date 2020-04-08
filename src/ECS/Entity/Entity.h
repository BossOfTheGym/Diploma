#pragma once

#include <entt/entt.hpp>

namespace ecs::entity
{
	using Entity                  = entt::entity;
	using EntityComponentRegistry = entt::registry;

	constexpr Entity null = entt::null;
}