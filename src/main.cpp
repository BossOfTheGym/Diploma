#include <iostream>
#include <memory>
#include <chrono>

#include <Simulator/Simulator.h>

#include <SpaceUtils/Lambert.h>

#include "Simulator/Components/SimData.h"
#include "Simulator/Components/Orbit.h"

void testSpaceUtils()
{
	
}

int main()
{
	testSpaceUtils();

	sim::Simulator sim;
	sim.mainLoop();

	return 0;
}