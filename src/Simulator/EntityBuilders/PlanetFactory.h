#pragma once

#include "entity_builders_common.h"

namespace sim
{
	class Simulator;

	class PlanetFactory : public ecs::entity::EntityBuilder<PlanetFactory>
	{
	public:
		using this_t = PlanetFactory;
		using base_t = ecs::entity::EntityBuilder<PlanetFactory>;


	public:
		PlanetFactory(ecs::entity::EntityBuilderManager* manager);

		virtual ~PlanetFactory() = default;


	public:
		virtual Entity buildEntity() override;

	private:
		Simulator* m_simulator{nullptr};
	};
}
