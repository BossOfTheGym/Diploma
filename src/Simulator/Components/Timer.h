#pragma once

#include <ECS/ecs_common.h>

namespace comp
{
	using Time = ecs::Time;

	struct Timer
	{
		Time duration{};
	};
}