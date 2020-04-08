#pragma once

#include "../ecs_common.h"

namespace ecs::util
{
	using ecs::Id;
	using ecs::BAD_ID;

	class StaticType
	{
	public:
		StaticType(Id id) : m_id(id)
		{}

		StaticType(const StaticType&) = default;
		StaticType(StaticType&&)      = default;

		~StaticType() = default;

		StaticType& operator = (const StaticType&) = default;
		StaticType& operator = (StaticType&&)      = default;

	public:
		Id getStaticTypeId() const
		{
			return m_id;
		}

	private:
		Id m_id{BAD_ID};
	};
}