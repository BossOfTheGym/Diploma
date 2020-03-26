#include "SphereMesh.h"

namespace meshes
{
	SphereMesh::SphereMesh(mesh::Mesh&& sphereMesh, Data&& sphereData)
		: mesh::Mesh(std::move(sphereMesh))
		, m_sphereData(std::move(sphereData))
	{}

	SphereMesh::SphereMesh(VertexArray&& vertexArray, const DrawInfo& info, bool usesIndices, Data&& sphereData)
		: mesh::Mesh(std::move(vertexArray), info, usesIndices)
		, m_sphereData(std::move(sphereData))
	{}

	SphereMesh::SphereMesh(SphereMesh&& another)
	{
		*this = std::move(another);
	}

	SphereMesh& SphereMesh::operator = (SphereMesh&& another)
	{
		static_cast<mesh::Mesh&>(*this) = std::move(another);

		std::swap(m_sphereData, another.m_sphereData);

		return *this;
	}


	const SphereMesh::Data& SphereMesh::getData() const
	{
		return m_sphereData;
	}
}