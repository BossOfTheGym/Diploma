#pragma once

#include <memory>

namespace basic_interface
{
	template<class Return>
	class ICopy
	{
	public:
		using Pointer = std::unique_ptr<Return>;

		[[nodiscard]]
		virtual Pointer copy() const = 0;
	};
}