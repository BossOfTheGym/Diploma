#pragma once

#include "entity_builders_common.h"

// TODO
namespace sim
{
	class PlayerFactory : public ecs::entity::EntityBuilder<PlayerFactory>
	{
	public:
		using this_t = PlayerFactory;
		using base_t = ecs::entity::EntityBuilder<PlayerFactory>;

			
	public:
		PlayerFactory(ecs::entity::EntityBuilderManager* manager);
		
		virtual ~PlayerFactory() = default;


	public:
		virtual Entity buildEntity() override;


	private:
		Simulator* m_simulator{nullptr};
	};
}