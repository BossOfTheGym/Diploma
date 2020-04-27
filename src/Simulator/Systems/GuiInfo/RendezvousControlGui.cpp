#include "RendezvousControlGui.h"

namespace sim
{
	RendezvousSystemInfo::RendezvousSystemInfo(ecs::sys::SystemManager* manager)
		: base_t(manager)
	{}

	void RendezvousSystemInfo::render()
	{
		// TODO or not TODO
		ImGui::Text("Hello from rendezvous control system");
	}

	const char* RendezvousSystemInfo::name() const
	{
		return "Rendezvous constrol system";
	}
}
