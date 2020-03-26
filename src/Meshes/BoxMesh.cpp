#include "BoxMesh.h"

#include <Math/MathLib.h>

namespace meshes
{
	BoxMesh::BoxMesh(mesh::Mesh&& another, Data&& boxData)
		: mesh::Mesh(std::move(another))
		, m_boxData(std::move(boxData))
	{}

	BoxMesh::BoxMesh(VertexArray&& vertexArray, const DrawInfo& info, bool usesIndices, Data&& boxData)
		: mesh::Mesh(std::move(vertexArray), info, usesIndices)
		, m_boxData(std::move(boxData))
	{}

	BoxMesh::BoxMesh(BoxMesh&& another)
	{
		*this = std::move(another);
	}

	BoxMesh& BoxMesh::operator = (BoxMesh&& another)
	{
		static_cast<mesh::Mesh&>(*this) = std::move(another);

		std::swap(m_boxData, another.m_boxData);

		return *this;
	}


	const BoxMesh::Data& BoxMesh::getData() const
	{
		return m_boxData;
	}
}
