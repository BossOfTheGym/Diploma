#pragma once

#include "entity_builders_common.h"

namespace sim
{
	class Simulator;

	class ChaserFactory : public ecs::entity::EntityBuilder<ChaserFactory>
	{
	public:
		using this_t = ChaserFactory;
		using base_t = ecs::entity::EntityBuilder<ChaserFactory>;


	public:
		ChaserFactory(ecs::entity::EntityBuilderManager* manager);

		virtual ~ChaserFactory() = default;


	public:
		virtual Entity buildEntity() override;

	private:
		Simulator* m_simulator{nullptr};
	};
}
