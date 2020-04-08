#pragma once

#include "../ecs_common.h"
#include "../Utility/StaticType.h"
#include "../Utility/NoCopyMove.h"

namespace ecs::sys
{
	using ecs::Float;
	using ecs::Id;


	class SystemManager;

	class ISystem : public ecs::util::NoCopyMove, public ecs::util::StaticType
	{
	public:
		ISystem(SystemManager* manager, Id staticTypeId);

		virtual ~ISystem() = default;


	public:
		virtual void update(Float t, Float dt) = 0;


	public:
		void disable();

		void enable();

		bool enabled() const;


		SystemManager* getSystemManager() const;


	private:
		bool m_enabled{true};

		SystemManager* m_manager{nullptr};
	};
}