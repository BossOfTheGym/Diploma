#pragma once

#include <Math/MathLib.h>

namespace space_utils
{
	using math::Vec3;
	using math::Float;
	using math::UInt32;
	using math::UInt64;

	using math::operator "" _FL;

	namespace stumpff
	{
		Float c0(Float x);

		Float c1(Float x);

		Float c2(Float x);

		Float c3(Float x);

		Float c4(Float x);

		Float c5(Float x);

		Float c6(Float x);


		// templated version
		template<UInt64 n>
		struct factorial
		{
			static_assert(n <= 20, "Factorial overflow will overflow if n > 20");

			static constexpr UInt64 value = n * factorial<n - 1>::value;
		};

		template<>
		struct factorial<0ULL>
		{
			static constexpr UInt64 value = 1ULL;
		};

		template<UInt64 n>
		constexpr UInt64 factorial_v = factorial<n>::value;


		template<UInt64 n>
		Float c(Float x)
		{
			static_assert(n <= 20, "Max n is 20");

			if (std::abs(x) > std::numeric_limits<Float>::epsilon())
			{
				return (1.0_FL / factorial_v<n> - c<n - 2>(x)) / x;
			}
			return 1.0_FL / factorial_v<n>;
		}

		template<UInt64 n>
		Float c<0>(Float x);

		template<>
		Float c<1>(Float x);
	}
}
