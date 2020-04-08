#pragma once

#include <Math/MathLib.h>
#include <ECS/ecs_common.h>

namespace comp
{
	using ecs::Id;

	struct MeshComponent
	{
		Id id{ecs::BAD_ID};
	};
}