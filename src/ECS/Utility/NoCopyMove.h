#pragma once


namespace ecs::util
{
	class NoCopyMove
	{
	public:
		NoCopyMove() = default;

		NoCopyMove(const NoCopyMove&) = delete;
		NoCopyMove(NoCopyMove&&)      = delete;

		~NoCopyMove() = default;

		NoCopyMove& operator = (const NoCopyMove&) = delete;
		NoCopyMove& operator = (NoCopyMove&&)      = delete;
	};
}