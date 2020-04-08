#include "IInputController.h"

namespace sim
{
	IInputController::IInputController(ecs::ECSEngine* engine)
		: m_engine(engine)
	{
		// TODO : assert
	}

	ecs::ECSEngine* IInputController::getEngineInstance() const
	{
		return m_engine;
	}
}