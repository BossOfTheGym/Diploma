#include "SimData.h"


namespace comp
{
	Vec3 SimData::radius() const
	{
		return Vec3(state[0], state[1], state[2]);
	}

	Vec3 SimData::velocity() const
	{
		return Vec3(state[3], state[4], state[5]);
	}
}