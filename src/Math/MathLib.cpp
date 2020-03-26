#include "MathLib.h"


namespace math
{
	Float operator "" _FL(long double value)
	{
		return static_cast<Float>(value);
	}
}