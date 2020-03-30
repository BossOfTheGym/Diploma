#pragma once

namespace ecs::util
{
	template<class T, class Id>
	class Family
	{
	public:
		template<class U>
		static Id id()
		{
			static const Id s_uId = s_counter++;
			
			return s_uId;
		}

		static Id count()
		{
			return s_counter;
		}

	private:
		static Id s_counter;
	};

	template<class T, class Id>
	Id Family<T, Id>::s_counter{};
}