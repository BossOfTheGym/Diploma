#pragma once

#include "entity_builders_common.h"

namespace sim
{
	class Simulator;

	class SatelliteFactory : public ecs::entity::EntityBuilder<SatelliteFactory>
	{
	public:
		using this_t = SatelliteFactory;
		using base_t = ecs::entity::EntityBuilder<SatelliteFactory>;


	public:
		SatelliteFactory(ecs::entity::EntityBuilderManager* manager);

		virtual ~SatelliteFactory() = default;;

	public:
		virtual Entity buildEntity() override;

	private:
		Simulator* m_simulator;
	};
}