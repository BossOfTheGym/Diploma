#pragma once

namespace ecs::comp
{
	class IComponent
	{
	public:
		virtual ~IComponent() = 0;
	};

	IComponent::~IComponent()
	{};
}