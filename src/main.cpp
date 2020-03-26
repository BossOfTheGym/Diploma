#include <iostream>

#include "Solvers/NumSolvers.h"
#include "Functions/Functions.h"

int main()
{
	auto [f, j] = funcs::gravitationForce<double, 3>(3);

	return 0;
}