#include "Stumpff.h"

namespace space_utils
{
	namespace stumpff
	{
		Float c0(Float x)
		{
			if (x > std::numeric_limits<Float>::epsilon())
			{
				return std::cos(std::sqrt(x));
			}
			else if (x < std::numeric_limits<Float>::epsilon())
			{
				return std::cosh(std::sqrt(-x));
			}
			return 1.0_FL;
		}

		Float c1(Float x)
		{
			if (x > std::numeric_limits<Float>::epsilon())
			{
				Float xSq = std::sqrt(x);

				return std::sin(xSq) / xSq;
			}
			else if (x < std::numeric_limits<Float>::epsilon())
			{
				Float xSq = std::sqrt(-x);

				return std::sinh(xSq) / xSq;
			}
			return 1.0_FL;
		}

		Float c2(Float x)
		{
			if (x > std::numeric_limits<Float>::epsilon())
			{
				return (1.0_FL - std::cos(std::sqrt(x))) / x;
			}
			else if (x < std::numeric_limits<Float>::epsilon())
			{
				return (std::cosh(std::sqrt(-x)) - 1.0_FL) / (-x);
			}
			return 1.0_FL / 2;
		}

		Float c3(Float x)
		{
			if (x > std::numeric_limits<Float>::epsilon())
			{
				Float sq = std::sqrt(x);

				return (sq - std::sin(sq)) / (x * sq);
			}
			else if (x < std::numeric_limits<Float>::epsilon())
			{
				Float sq = std::sqrt(-x);

				return (std::sinh(sq) - sq) / (-x * sq);
			}
			return 1.0_FL / 6;
		}

		Float c4(Float x)
		{
			if (std::abs(x) > std::numeric_limits<Float>::epsilon())
			{
				return (1.0_FL / 2 - c2(x)) / x;	
			}
			return 1.0_FL / 24;
		}

		Float c5(Float x)
		{
			if (std::abs(x) > std::numeric_limits<Float>::epsilon())
			{
				return (1.0_FL / 2 - c3(x)) / x;	
			}
			return 1.0_FL / 120;
		}

		Float c6(Float x)
		{
			if (std::abs(x) > std::numeric_limits<Float>::epsilon())
			{
				return (1.0_FL / 2 - c4(x)) / x;	
			}
			return 1.0_FL / 720;
		}


		template<>
		Float c<0>(Float x)
		{
			return c0(x);
		}

		template<>
		Float c<1>(Float x)
		{
			return c1(x);
		}
	}
}