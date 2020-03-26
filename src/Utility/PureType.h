#pragma once

#include <type_traits>

namespace util
{
	template<class T>
	using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<T>>;
}