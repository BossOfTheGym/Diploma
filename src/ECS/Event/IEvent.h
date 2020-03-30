#pragma once

namespace event
{
	class IEvent
	{
	public:
		virtual ~IEvent() = 0;
	};

	IEvent::~IEvent()
	{}
}