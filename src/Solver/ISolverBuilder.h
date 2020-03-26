#pragma once

#include "SolverWrapper.h"

namespace solver
{
	template<class Function, class Jacobian, class Time, class Value>
	class ISolverBuilder
	{
	public:
		virtual SolverWrapper<Function, Time, Value> build() const = 0;
	};
}