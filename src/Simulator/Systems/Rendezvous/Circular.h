#pragma once

#include "Method.h"

namespace sim
{	
	class ToCircular : public Method
	{
	public:
		ToCircular(RendezvousControlSystem* sys);

		virtual ~ToCircular() = default;


	public:
		virtual bool startRendezvous(Entity target, Entity chaser, Time t, Time dt) override;
	};
}