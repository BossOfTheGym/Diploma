#pragma once

#include <glWrap/OpenGL.h>

namespace gl
{
	class Id
	{
	public:
		enum : GLuint
		{
			Empty = 0U
		};

	public:
		Id();

		Id(GLuint id);

		Id(const Id&) = delete;
		Id(Id&& id);

		Id& operator = (const Id&) = delete;
		Id& operator = (Id&& id);

		~Id() = default;


	public:
		GLuint id() const;

		bool valid() const;


	protected:
		void resetId();


	private:
		GLuint m_id;
	};
}
