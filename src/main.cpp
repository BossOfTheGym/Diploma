#include <iostream>
#include <memory>
#include <chrono>

#include <Simulator/Simulator.h>

#include <SpaceUtils/Lambert.h>

void testSpaceUtils()
{
	
}

int main()
{
	//testSpaceUtils();
	sim::Simulator sim;
	sim.mainLoop();

	return 0;
}