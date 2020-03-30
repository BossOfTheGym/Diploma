#pragma once

#include "ISystem.h"

#include <unordered_map>
#include <unordered_set>
#include <memory>


namespace ecs::sys
{
	class SystemManager
	{
	public:
		using Register     = std::unordered_map<Id, std::unique_ptr<ISystem>>;
		using Dependencies = std::unordered_set<Id, std::unordered_set<Id>>
		
	private:
		
		Register m_systemRegister;
	};
}