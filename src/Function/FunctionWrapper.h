#pragma once

#include <functional>

#include "IFunction.h"


namespace func
{
	template<class>
	class FunctionWrapper;

	template<class Ret, class ... Args>
	class FunctionWrapper<Ret(Args...)> : public IFunction<Ret(Args...)>
	{
	public:
		using Function       = std::function<Ret(Args...)>;
		using FunctionHolder = std::shared_ptr<Function>;

		using BaseInterface = IFunction<Ret(Args...)>;
		using Pointer       = typename BaseInterface::Pointer;
		

	public:
		template<class FunctionT>
		FunctionWrapper(FunctionT function)
			: m_holder(new Function(std::forward<FunctionT>(function)))
		{}

		FunctionWrapper() = default;
		FunctionWrapper(const FunctionWrapper&) = default;
		FunctionWrapper(FunctionWrapper&&)      = default;

		virtual ~FunctionWrapper() = default;


		template<class FunctionT>
		FunctionWrapper& operator = (FunctionT function)
		{
			m_holder.reset(new Function(std::forward<FunctionT>(function)));

			return *this;
		}

		FunctionWrapper& operator = (const FunctionWrapper& function) = default;
		FunctionWrapper& operator = (FunctionWrapper&& function)      = default;


	public:
		// IFunction
		virtual Ret operator() (Args ... args) override
		{
			return (*m_holder)(args...);
		}

		virtual Ret operator() (Args ... args) const override
		{
			return (*m_holder)(args...);
		}

		// ICopy
		[[nodiscard]]
		virtual Pointer copy() const override
		{
			return Pointer(new FunctionWrapper(*this));
		}


	private:
		FunctionHolder m_holder;
	};
}
