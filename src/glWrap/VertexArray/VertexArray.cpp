#include "VertexArray.h"

#include <utility>

#include <glWrap/Buffer/Buffer.h>


namespace gl
{
	//constructors & destructor
	VertexArray::VertexArray()
		: Id()
	{}

	VertexArray::VertexArray(VertexArray&& vertexBuffer) : Id(std::move(vertexBuffer))
	{}


	VertexArray::~VertexArray()
	{
		deleteArrayBuffer();
	}


	//operators
	VertexArray& VertexArray::operator = (VertexArray&& vertexBuffer)
	{
		static_cast<Id&>(*this) = static_cast<Id&&>(vertexBuffer);

		return *this;
	}


	//delete & reset
	void VertexArray::deleteArrayBuffer()
	{
		GLuint vertexArrayId = id();
		glDeleteVertexArrays(1, &vertexArrayId);
		resetArrayBuffer();
	}


	void VertexArray::resetArrayBuffer()
	{
		resetId();
	}


	//core functions
	void VertexArray::genArray()
	{
		GLuint arrayID;

		//creates only name, doesn't create inner state
		glGenVertexArrays(1, &arrayID);

		static_cast<Id&>(*this) = arrayID;
	}

	void VertexArray::createArray()
	{
		GLuint arrayID;

		//creates name and inner object's state
		glCreateVertexArrays(1, &arrayID);

		static_cast<Id&>(*this) = arrayID;
	}


	void VertexArray::bind() const
	{
		glBindVertexArray(id());
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(Id::Empty);

		glBindBuffer(static_cast<GLenum>(BufferTarget::ArrayBuffer), Id::Empty);
		glBindBuffer(static_cast<GLenum>(BufferTarget::ElementArrayBuffer), Id::Empty);
	}


	void VertexArray::enableAttribArray(GLuint index)
	{
		glEnableVertexAttribArray(index);
	}

	void VertexArray::vertexAttribPointer(const PointerInfo& info)
	{
		glVertexAttribPointer(
			info.index
			, static_cast<GLint>(info.size)
			, static_cast<GLenum>(info.type)
			, static_cast<GLboolean>(info.normalized)
			, info.stride
			, info.pointer
		);
	}

	void VertexArray::vertexAttribIPointer(const PointerInfo& info)
	{
		glVertexAttribIPointer(
			info.index
			, static_cast<GLint>(info.size)
			, static_cast<GLenum>(info.type)
			, info.stride
			, info.pointer
		);
	}

	void VertexArray::vertexAttribLPointer(const PointerInfo & info)
	{
		glVertexAttribLPointer(
			info.index
			, static_cast<GLint>(info.size)
			, static_cast<GLenum>(DataType::Double)
			, info.stride
			, info.pointer
		);
	}

	void VertexArray::disableAttribArray(GLuint index)
	{
		glDisableVertexAttribArray(index);
	}


	void VertexArray::drawArrays(const DrawInfo& info) const
	{
		glDrawArrays(
			static_cast<GLenum>(info.drawMode)
			, info.first
			, info.elementsCount
		);
	}

	void VertexArray::drawElements(const DrawInfo& info) const
	{
		glDrawElements(
			static_cast<GLenum>(info.drawMode)
			, info.elementsCount
			, static_cast<GLenum>(info.indicesType)
			, info.indices
		);
	}


	//misc
	void VertexArray::setAttribPointer(const PointerInfo& info)
	{
		enableAttribArray(info.index);
		vertexAttribPointer(info);
	}

	void VertexArray::setAttribIPointer(const PointerInfo& info)
	{
		enableAttribArray(info.index);
		vertexAttribIPointer(info);
	}

	void VertexArray::setAttribLPointer(const PointerInfo & info)
	{
		enableAttribArray(info.index);
		vertexAttribLPointer(info);
	}


	void VertexArray::setAttribPointerInBuffer(const Buffer& buffer, const PointerInfo& info)
	{
		buffer.bind(BufferTarget::ArrayBuffer);

		setAttribPointer(info);

		buffer.unbind(BufferTarget::ArrayBuffer);
	}

	void VertexArray::setAttribIPointerInBuffer(const Buffer& buffer, const PointerInfo& info)
	{
		buffer.bind(BufferTarget::ArrayBuffer);

		setAttribIPointer(info);

		buffer.unbind(BufferTarget::ArrayBuffer);
	}

	void VertexArray::setAttribLPointerInBuffer(const Buffer & buffer, const PointerInfo & info)
	{
		buffer.bind(BufferTarget::ArrayBuffer);

		setAttribLPointer(info);

		buffer.unbind(BufferTarget::ArrayBuffer);
	}


	void VertexArray::setElementsBuffer(const Buffer& buffer)
	{
		buffer.bind(BufferTarget::ElementArrayBuffer);
	}	
}