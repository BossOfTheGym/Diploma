#include "Method.h"

namespace sim
{
	Method::Method(RendezvousControlSystem* sys)
		: m_sys(sys)
	{
		// TODO : assert
	}

	RendezvousControlSystem* Method::getSystem() const
	{
		return m_sys;
	}
}