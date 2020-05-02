#pragma once

#include "Method.h"

// not used

namespace sim
{	
	class ToCircular : public TwoImpulsMethod
	{
	public:
		ToCircular(RendezvousControlSystem* sys);

		virtual ~ToCircular() = default;


	public:
		virtual Data startRendezvous(Entity target, Entity chaser, Time t, Time dt) override;
	};
}