#pragma once

#include "IMesh.h"

namespace mesh
{
	class Mesh : public IMesh
	{
	public:
		using VertexArray = IMesh::VertexArray;
		using DrawInfo    = IMesh::DrawInfo;


	public:
		Mesh();

		Mesh(VertexArray&& vertexArray, const DrawInfo& drawInfo, bool usesIndices);

		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&);

		virtual ~Mesh() = default;

		Mesh& operator = (const Mesh&) = delete;
		Mesh& operator = (Mesh&&);


	public:
		virtual const VertexArray& getVertexArray() const override;

		virtual const DrawInfo& getDrawInfo() const override;

		virtual bool usingIndices() const override;


	protected:
		VertexArray m_vertexArray;
		DrawInfo    m_drawInfo;

		bool m_usesIndices;
	};
}