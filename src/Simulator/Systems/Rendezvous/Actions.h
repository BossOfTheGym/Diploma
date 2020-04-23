#pragma once

#include <ECS/ecs_common.h>
#include <ECS/Utility/StaticType.h>

#include "../../Components/Rendezvous.h"

namespace sim
{
	using ecs::Id;
	using ecs::entity::Entity;

	using ecs::BAD_ID;
	using ecs::entity::null;
	
	using ecs::Time;

	class RendezvousControlSystem;

	class IAction : public ecs::util::StaticType
	{
	public:
		IAction(RendezvousControlSystem* sys, Id staticTypeId);

	public:
		virtual void update(Entity e, Time t, Time dt) = 0;

	public:
		RendezvousControlSystem* getSystem() const;

	private:
		RendezvousControlSystem* m_sys{nullptr};
	};


	template<class T>
	class BaseAction : public IAction
	{
	public:
		static const Id TYPE_ID;

	public:
		BaseAction(RendezvousControlSystem* sys)
			: IAction(sys, TYPE_ID)
		{}

	public:
		virtual void update(Entity e, Time t, Time dt) override
		{}
	};

	template<class T>
	const Id BaseAction<T>::TYPE_ID = T::TYPE_ID;


	class ImpulsAction : public BaseAction<comp::Impuls>
	{
	public:
		ImpulsAction(RendezvousControlSystem* sys);

	public:
		virtual void update(Entity actionList, Time t, Time dt) override;
	};

	class WaitAction : public BaseAction<comp::Wait>
	{
	public:
		WaitAction(RendezvousControlSystem* sys);

	public:
		virtual void update(Entity actionList, Time t, Time dt) override;
	};
}