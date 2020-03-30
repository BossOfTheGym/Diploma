#include "IInputController.h"

namespace sim
{
	IInputController::IInputController(Simulator* host)
		: m_host(host)
	{}

	Simulator* IInputController::getHost() const
	{
		return m_host;
	}
}