#pragma once

#include "IFunction.h"

namespace func
{
	template<class>
	class FunctionHolder;

	template<class Ret, class ... Args>
	class FunctionHolder<Ret(Args...)> : public IFunction<Ret(Args...)>
	{
	public:
		using this_t = FunctionHolder<Ret(Args...)>;
		using base_t = IFunction<Ret(Args...)>;

		using FunctionPtr = std::shared_ptr<base_t>;

		using Pointer = typename base_t::Pointer;


	public:
		FunctionHolder(base_t* functionPtr = nullptr)
			: m_holder(functionPtr)
		{}

		FunctionHolder(FunctionPtr functionPtr)
			: m_holder(functionPtr)
		{}

		FunctionHolder(const FunctionHolder&) = default;
		FunctionHolder(FunctionHolder&&)      = default;

		virtual ~FunctionHolder() = default;

		FunctionHolder& operator = (const FunctionHolder&) = default;
		FunctionHolder& operator = (FunctionHolder&&)      = default;


	public: // ICopy
		[[nodiscard]]
		virtual Pointer copy() const override
		{
			return Pointer(new FunctionHolder(*this));
		}


	public:
		virtual Ret operator() (Args ... args) override
		{
			return (*m_holder)(args...);
		}

		virtual Ret operator() (Args ... args) const override
		{
			return (*m_holder)(args...);
		}


	private:
		FunctionPtr m_holder{};
	};
}