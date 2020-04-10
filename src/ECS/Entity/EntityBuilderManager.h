#pragma once

#include "IEntityBuilder.h"

#include <memory>
#include <utility>
#include <unordered_map>

namespace ecs
{
	class ECSEngine;
}

namespace ecs::entity
{
	using ecs::Id;
	using ecs::BAD_ID;

	class EntityBuilderManager : public ecs::util::NoCopyMove
	{
	public:
		using BuildersRegistry = std::unordered_map<Id, std::unique_ptr<IEntityBuilder>>;


	public:
		EntityBuilderManager(ecs::ECSEngine* engineInstance);

		~EntityBuilderManager() = default;


	public:
		template<class Builder, class ... Args>
		void add(Args&& ... args)
		{
			auto ptr = std::make_unique<Builder>(std::forward<Args>(args)...);
			m_registry[Builder::TYPE_ID] = std::move(ptr);
		}

		template<class Builder>
		void remove()
		{
			m_registry.erase(Builder::TYPE_ID);
		}

		template<class Builder>
		Builder* get()
		{
			Builder* ptr{nullptr};
			if (auto it = m_registry.find(Builder::TYPE_ID); it != m_registry.end())
			{
				ptr = static_cast<Builder*>((*it).second.get());
			}
			return ptr;
		}


		ecs::ECSEngine* getEngineInstance() const;

	private:
		ecs::ECSEngine*  m_engine{nullptr};
		BuildersRegistry m_registry{};
	};
}