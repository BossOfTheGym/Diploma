#pragma once

#include "../ecs_common.h"
#include "../Utility/StaticType.h"
#include "../Utility/NoCopyMove.h"

namespace ecs::sys
{
	using ecs::Time;
	using ecs::Id;


	class SystemManager;

	class ISystem : public ecs::util::NoCopyMove, public ecs::util::StaticType
	{
	public:
		ISystem(SystemManager* manager, Id staticTypeId);

		virtual ~ISystem() = default;


	public:
		virtual void update(Time t, Time dt) = 0;


	public:
		SystemManager* getSystemManager() const;

	private:
		SystemManager* m_manager{nullptr};
	};
}