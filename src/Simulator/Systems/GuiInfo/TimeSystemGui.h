#pragma once

#include "GuiInfo.h"

#include "../TimeSystem.h"

namespace sim
{
	class TimeSystemInfo : public SystemInfo<TimeSystem, TimeSystemInfo>
	{
	public:
		using this_t = TimeSystemInfo;
		using base_t = SystemInfo<TimeSystem, TimeSystemInfo>;

	public:
		TimeSystemInfo(ecs::sys::SystemManager* manager);

	public:
		virtual void render() override;

		virtual const char* name() const override;
	};
}