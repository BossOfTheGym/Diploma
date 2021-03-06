#pragma once

#include "ISystem.h"
#include "../Utility/NoCopyMove.h"

#include <GL/glew.h>

#include <unordered_map>
#include <vector>
#include <utility>
#include <memory>

namespace ecs
{
	class ECSEngine;
}

namespace ecs::sys
{
	// TODO : make update order
	class SystemManager : public ecs::util::NoCopyMove
	{
	public:
		using Registry      = std::unordered_map<Id, std::unique_ptr<ISystem>>;
		using DependencyMat = std::vector<std::vector<char>>;

	public:
		SystemManager(ecs::ECSEngine* engine);

		~SystemManager() = default;


	public:
		template<class System, class Dependency>
		void addDependency()
		{
			auto sysId = System::TYPE_ID;
			auto depId = Dependency::TYPE_ID;

			m_dependencyMat[depId][sysId] = true;
		}

		template<class System, class Dependency>
		void removeDependency()
		{
			auto sysId = System::TYPE_ID;
			auto depId = Dependency::TYPE_ID;

			m_dependencyMat[sysId][depId] = false;
		}

		template<class System, class ... Args>
		void add(Args&& ... args)
		{
			auto ptr = std::make_unique<System>(this, std::forward<Args>(args)...);

			m_systemRegistry[System::TYPE_ID] = std::move(ptr);
		}

		template<class System>
		System* get() const
		{
			auto sysId = System::TYPE_ID;

			System* sys = nullptr;
			if (auto it = m_systemRegistry.find(sysId); it != m_systemRegistry.end())
			{
				sys = static_cast<System*>((*it).second.get());
			}
			return sys;
		}


		ecs::ECSEngine* getECSEngine() const;


	private:
		Registry      m_systemRegistry{};
		DependencyMat m_dependencyMat{};

		ECSEngine* m_engine;
	};
}