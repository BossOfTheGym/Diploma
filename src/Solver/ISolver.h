#pragma once

#include "../Interface/ICopy.h"

namespace solver
{
	template<class Function, class Jacobian, class Time, class Value>
	class ISolver : public basic_interface::ICopy<ISolver<Function, Jacobian, Time, Value>>
	{
	public:
		using BaseInterface = basic_interface::ICopy<ISolver<Function, Jacobian, Time, Value>>;
		using Pointer       = typename BaseInterface::Pointer;

		virtual Value solve(const Function& f, const Jacobian& j, Time t, Value u, Time dt) = 0;
	};
}