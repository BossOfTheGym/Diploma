#pragma once

#include <memory>

#include "IMesh.h"

namespace mesh
{
	class IMeshBuilder
	{
	public:
		[[nodiscard]]
		virtual IMesh* buildMesh() = 0;
	};
}