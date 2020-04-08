#pragma once

#include <Mesh/Mesh.h>

#include <glWrap/Buffer/Buffer.h>


namespace meshes
{
	class QuadMesh : public mesh::Mesh
	{
	public:
		struct Data
		{
			gl::Buffer positions{};
		};


	public:
		QuadMesh(mesh::Mesh&& another, Data&& boxData);

		QuadMesh(VertexArray&& vertexArray, const DrawInfo& info, bool usesIndices, Data&& boxData);

		QuadMesh(const QuadMesh&) = delete;
		QuadMesh(QuadMesh&&);

		virtual ~QuadMesh() = default;

		QuadMesh& operator = (const QuadMesh&) = delete;
		QuadMesh& operator = (QuadMesh&&);


	public:
		const Data& getData() const;


	private:
		Data m_data;
	};
}