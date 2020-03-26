#include "BoxMeshBuilder.h"

#include "BoxMesh.h"

#include <glWrap/Buffer/Buffer.h>
#include <Math/MathLib.h>


namespace meshes
{
	using math::Vec3f32;
	using math::UInt32;

	BoxMeshBuilder::BoxMeshBuilder(const AttributeInfo& info)
		: m_attributeInfo(info)
	{}
	
	[[nodiscard]]
	mesh::IMesh* BoxMeshBuilder::buildMesh() const
	{
		Vec3f32 vertices[8] =
		{
			  Vec3f32(-1.0f, -1.0f, -1.0)
			, Vec3f32(-1.0f, -1.0f, +1.0)
			, Vec3f32(-1.0f, +1.0f, -1.0)
			, Vec3f32(-1.0f, +1.0f, +1.0)
			, Vec3f32(+1.0f, -1.0f, -1.0)
			, Vec3f32(+1.0f, -1.0f, +1.0)
			, Vec3f32(+1.0f, +1.0f, -1.0)
			, Vec3f32(+1.0f, +1.0f, +1.0)
		};

		UInt32 indices[36] =
		{
			  3, 7, 2
			, 6, 2, 7

			, 6, 7, 4
			, 5, 4, 7

			, 7, 3, 5
			, 1, 5, 3

			, 3, 2, 1
			, 0, 1, 2

			, 2, 6, 0
			, 4, 0, 6

			, 4, 5, 0
			, 1, 0, 5
		};

		BoxMesh::Data boxData{};
		boxData.positions.createBuffer();
		boxData.positions.namedBufferData(sizeof(vertices), vertices, gl::BufferUsage::StaticDraw);

		boxData.indices.createBuffer();
		boxData.indices.namedBufferData(sizeof(indices), indices, gl::BufferUsage::StaticDraw);

		gl::VertexArray vertexArray;
		vertexArray.createArray();
		vertexArray.bind();
		vertexArray.setAttribPointerInBuffer(
			boxData.positions
			, gl::VertexArray::PointerInfo
			{
				  static_cast<GLuint>(m_attributeInfo.position)
				, gl::AttributeSize::Three
				, gl::DataType::Float
				, gl::GLBool::True
				, static_cast<GLsizei>(0)
				, nullptr
			}
		);
		vertexArray.setElementsBuffer(boxData.indices);
		vertexArray.unbind();

		mesh::Mesh::DrawInfo drawInfo{};
		drawInfo.drawMode      = gl::DrawMode::Triangles;
		drawInfo.first         = static_cast<GLint>(0);
		drawInfo.elementsCount = static_cast<GLsizei>(36);
		drawInfo.indicesType   = gl::IndicesType::UnsignedInt;

		return new BoxMesh(std::move(vertexArray), drawInfo, true, std::move(boxData));
	}
}