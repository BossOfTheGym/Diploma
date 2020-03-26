#pragma once

#include <glWrap/OpenGL.h>
#include <glWrap/ID/Id.h>


namespace gl
{
	class Buffer : public Id
	{
	public:
		Buffer();

		Buffer(Buffer&& buffer);

		~Buffer();


		Buffer& operator = (Buffer&& buffer);


	public:
		void genBuffer();

		void createBuffer();


		void bind(BufferTarget target) const;

		void unbind(BufferTarget target) const;


		void deleteBuffer();


		void bufferData(BufferTarget target, GLsizeiptr pSize, const GLvoid* data, BufferUsage pUsage);

		void bufferSubData(BufferTarget target, GLintptr offset, GLsizei size, const GLvoid* data);

		void namedBufferData(GLsizeiptr pSize, const GLvoid* data, BufferUsage pUsage);

		void namedBufferSubData(GLintptr offset, GLsizei size, const GLvoid* data);


		//only AtomicCounterBuffer, TransformFeedbackBuffer, UniformBuffer, ShaderStorageBuffer allowed
		void bindBufferBase(IndexBufferTarget target, GLuint index);

		void bindBufferRange(IndexBufferTarget target, GLuint index, GLintptr offset, GLsizeiptr size);

		void unbindBufferBase(IndexBufferTarget target, GLuint index);


		GLint getBufferParameteriv();

		GLint64 getBufferParameter64v();


		GLsizeiptr size() const;

		BufferUsage usage() const;


	private:
		void resetBuffer();


	private:
		GLsizeiptr  m_size;
		BufferUsage m_usage;
	};
}