#include "InputController.h"

#include "../../Simulator.h"

namespace sim
{
	InputController::InputController(Simulator* host) 
		: IInputController(host)
	{}
	
	void InputController::mouseEvent(BaseWindow& window, double xPos, double yPos)
	{
		
	}

	void InputController::mouseButtonEvent(BaseWindow & window, int button, int action, int mods)
	{
		
	}

	void InputController::scrollEvent(BaseWindow & window, double xOffset, double yOffset)
	{
		
	}

	void InputController::keyEvent(BaseWindow & window, int key, int scancode, int action, int mods)
	{
		
	}
}