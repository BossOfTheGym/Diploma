#include "Mesh.h"

#include <utility>


namespace mesh
{
	Mesh::Mesh()
	{}

	Mesh::Mesh(VertexArray && vertexArray, const DrawInfo & drawInfo, bool usesIndices)
		: m_vertexArray(std::move(vertexArray))
		, m_drawInfo(drawInfo)
		, m_usesIndices(usesIndices)
	{}

	Mesh::Mesh(Mesh&& another) : Mesh()
	{
		*this = std::move(another);
	}

	Mesh& Mesh::operator = (Mesh&& another)
	{
		std::swap(m_vertexArray, another.m_vertexArray);
		std::swap(m_drawInfo, another.m_drawInfo);
		std::swap(m_usesIndices, another.m_usesIndices);

		return *this;
	}


	const Mesh::VertexArray& Mesh::getVertexArray() const
	{
		return m_vertexArray;
	}

	const Mesh::DrawInfo& Mesh::getDrawInfo() const
	{
		return m_drawInfo;
	}

	bool Mesh::usingIndices() const
	{
		return m_usesIndices;
	}
}