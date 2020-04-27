#include "TimeSystemGui.h"

namespace sim
{
	TimeSystemInfo::TimeSystemInfo(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{}

	void TimeSystemInfo::render()
	{
		auto* sysManager = getSystemManager();
		auto* engine     = sysManager->getECSEngine();
		auto* timeSystem = sysManager->get<TimeSystem>();

		std::uint64_t minWarp = timeSystem->getMinWarp();
		std::uint64_t maxWarp = timeSystem->getMaxWarp();
		std::uint64_t warp    = timeSystem->getWarp();
		ImGui::SliderScalar("Time warp", ImGuiDataType_U64, &warp, &minWarp, &maxWarp);

		float realTime = ecs::toSeconds<float>(timeSystem->getRealTime()).count();
		float simTime  = ecs::toSeconds<float>(timeSystem->getTime()).count();
		ImGui::Text("Real time: %.2fs", realTime);
		ImGui::Text("Sim. time: %.2fs", simTime);
		timeSystem->setWarp(warp);
	}

	const char* TimeSystemInfo::name() const
	{
		return "Time system";
	}
}