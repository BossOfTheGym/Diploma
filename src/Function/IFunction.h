#pragma once

#include "../Interface/ICopy.h"

namespace func
{
	template<class>
	class IFunction;

	template<class Ret, class ... Args>
	class IFunction<Ret(Args...)> : public basic_interface::ICopy<IFunction<Ret(Args...)>>
	{
	public:
		using BaseInterface = basic_interface::ICopy<IFunction<Ret(Args...)>>;
		using Pointer       = typename BaseInterface::Pointer; 


		virtual Ret operator()(Args ... args) = 0;

		virtual Ret operator()(Args ... args) const = 0;
	};
}