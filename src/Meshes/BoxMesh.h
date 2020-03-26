#pragma once

#include "../Mesh/Mesh.h"

#include <glWrap/Buffer/Buffer.h>

namespace meshes
{
	class BoxMesh : public mesh::Mesh
	{
	public:
		struct Data
		{
			gl::Buffer positions{};
			gl::Buffer indices{};
		};


	public:
		BoxMesh(mesh::Mesh&& another, Data&& boxData);

		BoxMesh(VertexArray&& vertexArray, const DrawInfo& info, bool usesIndices, Data&& boxData);

		BoxMesh(BoxMesh&&);
		BoxMesh(const BoxMesh&) = delete;

		virtual ~BoxMesh() = default;

		BoxMesh& operator = (BoxMesh&&);
		BoxMesh& operator = (const BoxMesh&) = delete;


	public:
		const Data& getData() const;


	private:
		Data m_boxData;
	};
}
