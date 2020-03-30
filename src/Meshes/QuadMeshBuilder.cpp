#include "QuadMeshBuilder.h"

#include <Math/MathLib.h>

#include <glWrap/Buffer/Buffer.h>
#include <glWrap/VertexArray/VertexArray.h>

#include "QuadMesh.h"

namespace meshes
{
	using math::Vec3f32;

	QuadMeshBuilder::QuadMeshBuilder(const AttributeInfo& info)
		: m_attributeInfo(info)
	{}


	mesh::IMesh* QuadMeshBuilder::buildMesh()
	{
		Vec3f32 vertices[6] = {
			  Vec3f32{+1.0f, +1.0f, 0.0f}
			, Vec3f32{-1.0f, +1.0f, 0.0f}
			, Vec3f32{-1.0f, -1.0f, 0.0f}
				  
			, Vec3f32{-1.0f, -1.0f, 0.0f}
			, Vec3f32{+1.0f, -1.0f, 0.0f}
			, Vec3f32{+1.0f, +1.0f, 0.0f}
		};

		QuadMesh::Data data;
		data.positions.createBuffer();
		data.positions.namedBufferData(sizeof(vertices), vertices, gl::BufferUsage::StaticDraw);
		
		gl::VertexArray vao;
		vao.createArray();
		vao.bind();
		vao.setAttribPointerInBuffer(
			data.positions
			, {
				m_attributeInfo.position
				, gl::AttributeSize::Three
				, gl::DataType::Float
				, gl::GLBool::True
				, 0
				, 0
			}
		);
		vao.unbind();

		mesh::IMesh::DrawInfo drawInfo{};
		drawInfo.drawMode      = gl::DrawMode::Triangles;
		drawInfo.first         = static_cast<GLint>(0);
		drawInfo.elementsCount = static_cast<GLsizei>(6);
		drawInfo.indicesType   = gl::IndicesType::None;

		return new QuadMesh(std::move(vao), drawInfo, false, std::move(data));
	}
}
