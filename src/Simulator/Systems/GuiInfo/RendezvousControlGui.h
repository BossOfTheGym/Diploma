#pragma once

#include "GuiInfo.h"

#include "../RendezvousControlSystem.h"

namespace sim
{
	class RendezvousSystemInfo : public SystemInfo<RendezvousControlSystem, RendezvousSystemInfo>
	{
	public:
		using this_t = RendezvousSystemInfo;
		using base_t = SystemInfo<RendezvousControlSystem, RendezvousSystemInfo>;

	public:
		RendezvousSystemInfo(ecs::sys::SystemManager* manager);

	public:
		virtual void render() override;

		virtual const char* name() const override;
	};
}