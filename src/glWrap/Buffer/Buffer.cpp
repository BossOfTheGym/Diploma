#include "Buffer.h"

#include <utility>


namespace gl
{
//constructors & destructor
	Buffer::Buffer()
		: Id()
		, m_size(0)
		, m_usage(BufferUsage::None)
	{}

	Buffer::Buffer(Buffer&& buffer) : Id()
	{
		*this = std::move(buffer);
	}

	Buffer::~Buffer()
	{
		deleteBuffer();
	}


	//operators
	Buffer& Buffer::operator = (Buffer&& buffer)
	{
		static_cast<Id&>(*this) = static_cast<Id&&>(buffer);

		std::swap(m_size, buffer.m_size);
		std::swap(m_usage, buffer.m_usage);

		return *this;
	}


	//core
	void Buffer::genBuffer()
	{
		GLuint bufferId;

		//generates only name(id) but not the contents(buffers inner state)
		glGenBuffers(1, &bufferId);
		static_cast<Id&>(*this) = bufferId;
	}

	void Buffer::createBuffer()
	{
		GLuint bufferID;

		//generates name(id) and contents(buffers inner state)
		glCreateBuffers(1, &bufferID);

		static_cast<Id&>(*this) = bufferID;
	}


	void Buffer::bind(BufferTarget target) const
	{
		glBindBuffer(static_cast<GLenum>(target), id());
	}

	void Buffer::unbind(BufferTarget target) const
	{
		glBindBuffer(static_cast<GLenum>(target), Id::Empty);
	}


	void Buffer::bufferData(BufferTarget target, GLsizeiptr size, const GLvoid* data, BufferUsage usage)
	{
		m_usage = usage;
		m_size  = size;

		glBufferData(static_cast<GLenum>(target), size, data, static_cast<GLenum>(usage));
	}

	void Buffer::bufferSubData(BufferTarget target, GLintptr offset, GLsizei size, const GLvoid* data)
	{
		glBufferSubData(static_cast<GLenum>(target), offset, size, data);
	}

	void Buffer::namedBufferSubData(GLintptr offset, GLsizei size, const GLvoid* data)
	{
		glNamedBufferSubData(id(), offset, size, data);
	}

	void Buffer::namedBufferData(GLsizeiptr size, const GLvoid* data, BufferUsage usage)
	{
		m_usage = usage;
		m_size  = size;

		glNamedBufferData(id(), size, data, static_cast<GLenum>(usage));
	}


	void Buffer::deleteBuffer()
	{
		GLuint bufferId = id();
		
		glDeleteBuffers(1, &bufferId);

		resetBuffer();
	}


	void Buffer::bindBufferBase(IndexBufferTarget target, GLuint index)
	{
		glBindBufferBase(static_cast<GLenum>(target), index, id());
	}

	void Buffer::bindBufferRange(IndexBufferTarget target, GLuint index, GLintptr offset, GLsizeiptr size)
	{
		glBindBufferRange(static_cast<GLenum>(target), index, id(), offset, size);
	}

	void Buffer::unbindBufferBase(IndexBufferTarget target, GLuint index)
	{
		glBindBufferBase(static_cast<GLenum>(target), index, Id::Empty);
	}


	GLint Buffer::getBufferParameteriv()
	{
		//TODO
		return {};
	}

	GLint64 Buffer::getBufferParameter64v()
	{
		//TODO
		return {};
	}


	//private
	void Buffer::resetBuffer()
	{
		resetId();

		m_usage  = BufferUsage::None;
		m_size = 0;
	}


	//get
	GLsizeiptr Buffer::size() const
	{
		return m_size;
	}

	BufferUsage Buffer::usage() const
	{
		return m_usage;
	}
}