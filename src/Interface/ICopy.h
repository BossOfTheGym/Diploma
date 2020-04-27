#pragma once

#include <memory>

namespace basic_interface
{
	template<class Return>
	class ICopy
	{
	public:
		virtual ~ICopy() = default;

	public:
		using Pointer = Return*;//std::unique_ptr<Return>;

		[[nodiscard]]
		virtual Pointer copy() const = 0;
	};
}