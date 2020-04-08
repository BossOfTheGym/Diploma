#include "QuadMesh.h"


namespace meshes
{
	QuadMesh::QuadMesh(mesh::Mesh&& another, Data&& data)
		: mesh::Mesh(std::move(another))
		, m_data(std::move(data))
	{}

	QuadMesh::QuadMesh(VertexArray&& vertexArray, const DrawInfo& info, bool usesIndices, Data&& boxData)
		: mesh::Mesh(std::move(vertexArray), info, usesIndices)
		, m_data(std::move(boxData))
	{}

	QuadMesh::QuadMesh(QuadMesh&& another) : mesh::Mesh(std::move(another))
	{
		std::swap(m_data, another.m_data);
	}

	QuadMesh& QuadMesh::operator = (QuadMesh&& another)
	{
		static_cast<mesh::Mesh&>(*this) = std::move(another);

		std::swap(m_data, another.m_data);

		return *this;
	}


	const QuadMesh::Data& QuadMesh::getData() const
	{
		return m_data;
	}
}