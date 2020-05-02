#include "Method.h"

namespace sim
{
	TwoImpulsMethod::TwoImpulsMethod(RendezvousControlSystem* sys)
		: m_sys(sys)
	{
		// TODO : assert
	}

	RendezvousControlSystem* TwoImpulsMethod::getSystem() const
	{
		return m_sys;
	}
}