#pragma once

#include <glWrap/OpenGl.h>
#include <glWrap/ID/Id.h>

namespace gl
{
	// #include <glWrap/Buffer/Buffer.h>
	class Buffer;

	class VertexArray : public Id
	{
	public:
		struct DrawInfo
		{
			DrawMode drawMode = DrawMode::None;        // drawArrays
			GLint    first    = static_cast<GLint>(0); // drawArrays

			GLsizei  elementsCount = static_cast<GLsizei>(0); // both drawArrays & drawElements

			IndicesType indicesType = IndicesType::None; // drawElements
			GLvoid* indices        = nullptr;           // drawElements
		};
		
		struct PointerInfo
		{
			GLuint        index      = static_cast<GLuint>(0);  // attribute index
			AttributeSize size       = AttributeSize::One;      // number of components per attribute(1,2,3,4)
			DataType      type       = DataType::None;          // data type
			GLBool        normalized = GLBool::False;           //
			GLsizei       stride     = static_cast<GLsizei>(0); // byte offset between attributes 
			GLvoid*       pointer    = nullptr;                 // offset of the first element
		};


	public:
		VertexArray();

		VertexArray(VertexArray&& vertexBuffer);

		~VertexArray();


		VertexArray& operator = (VertexArray&& vertexBuffer);


	public:
		void genArray();

		void createArray();

		void bind() const;

		void unbind() const;


		void enableAttribArray(GLuint index);

		void vertexAttribPointer(const PointerInfo& info);

		void vertexAttribIPointer(const PointerInfo& info);

		void vertexAttribLPointer(const PointerInfo& info);

		void disableAttribArray(GLuint index);


		void drawArrays(const DrawInfo& info) const;

		void drawElements(const DrawInfo& info) const;


		void deleteArrayBuffer();


	public:
		void setAttribPointer(const PointerInfo& ptrInfo);

		void setAttribIPointer(const PointerInfo& ptrInfo);

		void setAttribLPointer(const PointerInfo& ptrInfo);


		void setAttribPointerInBuffer(const Buffer& buffer, const PointerInfo& ptrInfo);

		void setAttribIPointerInBuffer(const Buffer& buffer, const PointerInfo& ptrInfo);

		void setAttribLPointerInBuffer(const Buffer& buffer, const PointerInfo& ptrInfo);


		void setElementsBuffer(const Buffer& buffer);


	private:
		void resetArrayBuffer();
	};

}