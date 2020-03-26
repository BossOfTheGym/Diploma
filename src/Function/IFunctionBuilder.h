#pragma once

#include "FunctionWrapper.h"

namespace func
{
	template<class>
	class IFunctionBuilder;


	template<class Ret, class ... Args>
	class IFunctionBuilder<Ret(Args...)>
	{
	public:
		using FunctionType = Ret(Args...);
		using ReturnType   = Ret;

	public:
		virtual FunctionWrapper<Ret(Args...)> build() const = 0;
	};
}