#pragma once

#include <Context/Context.h>
#include <ECS/Utility/NoCopyMove.h>

namespace ecs
{
	class ECSEngine;
}

namespace sim
{
	using context::IMediator;
	using context::BaseWindow;

	class IInputController : public IMediator, public ecs::util::NoCopyMove
	{
	public:
		IInputController(ecs::ECSEngine* engine);

		virtual ~IInputController() = default;


	public:
		ecs::ECSEngine* getEngineInstance() const;


	private:
		ecs::ECSEngine* m_engine{nullptr};
	};
}