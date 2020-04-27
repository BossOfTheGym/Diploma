#include <iostream>
#include <memory>
#include <chrono>

#include <Simulator/Simulator.h>

int main()
{
	sim::Simulator sim;
	sim.mainLoop();

	return 0;
}