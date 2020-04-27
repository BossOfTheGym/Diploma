#pragma once

#include "GuiInfo.h"

#include "../SimulatorState.h"


namespace sim
{
	class SimulatorStateGui : public SystemInfo<SimulatorState, SimulatorStateGui>
	{
	public:
		using this_t = SimulatorStateGui;
		using base_t = SystemInfo<SimulatorState, SimulatorStateGui>;


	public:
		SimulatorStateGui(ecs::sys::SystemManager* manager);


	public:
		virtual void render() override;

		virtual const char* name() const override;


	private:
		double m_transferTime{};
	};
}