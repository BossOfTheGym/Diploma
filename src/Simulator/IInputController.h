#pragma once

#include "../Context/Context.h"

namespace sim
{
	using context::IMediator;

	class Simulator;

	// owned by Simulator
	class IInputController : public IMediator
	{
	public:
		IInputController(Simulator* host);


	public:
		Simulator* getHost() const;


	private:
		Simulator* m_host{nullptr};
	};
}