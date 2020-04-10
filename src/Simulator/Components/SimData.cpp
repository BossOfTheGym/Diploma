#include "SimData.h"


namespace comp
{
	void SimData::setRadius(const Vec3& r)
	{
		state[0] = r[0];
		state[1] = r[1];
		state[2] = r[2];
	}

	void SimData::setVelocity(const Vec3& v)
	{
		state[3] = v[0];
		state[4] = v[1];
		state[5] = v[2];
	}

	Vec3 SimData::getRadius() const
	{
		return Vec3(state[0], state[1], state[2]);
	}

	Vec3 SimData::getVelocity() const
	{
		return Vec3(state[3], state[4], state[5]);
	}


	StateVec fromRadVel(const Vec3& r, const Vec3& v)
	{
		StateVec res{};
		res[0] = r.x;
		res[1] = r.y;
		res[2] = r.z;
		res[3] = v.x;
		res[4] = v.y;
		res[5] = v.z;
		return res;
	}
}