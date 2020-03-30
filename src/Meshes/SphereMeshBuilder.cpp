#include "SphereMeshBuilder.h"

#include <Math/MathLib.h>

#include "SphereMesh.h"

namespace meshes
{
	using math::F32;
	using math::F64;
	using math::Vec3f32;

	namespace
	{
		void pushSplitedTriangles(
			  std::vector<Vec3f32>& data
			, const Vec3f32& vertex0 // normalized
			, const Vec3f32& vertex1 // normalized
			, const Vec3f32& vertex2 // normalized
			, UInt32 split)
		{
			if (split > 0)
			{
				Vec3f32 vertex01 = glm::normalize((vertex0 + vertex1) / 2.0f);
				Vec3f32 vertex12 = glm::normalize((vertex1 + vertex2) / 2.0f);
				Vec3f32 vertex20 = glm::normalize((vertex2 + vertex0) / 2.0f);

				pushSplitedTriangles(data, vertex0, vertex01, vertex20, split - 1);

				pushSplitedTriangles(data, vertex01, vertex1, vertex12, split - 1);

				pushSplitedTriangles(data, vertex12, vertex2, vertex20, split - 1);

				pushSplitedTriangles(data, vertex01, vertex12, vertex20, split - 1);
			}
			else
			{
				data.push_back(vertex0);
				data.push_back(vertex1);
				data.push_back(vertex2);
			}
		}

		auto sphere(UInt32 split)
		{
			//icosahedron params
			const int PENTAGON_SIDES = 5;

			const F64 HALF_ANGLE = math::PI / PENTAGON_SIDES;
			const F64 ANGLE      = 2 * HALF_ANGLE;
			const F64 SIN_HALF   = std::sin(HALF_ANGLE);
				  
			const F64 ICOSAHEDRON_SIDE = std::sqrt(4 * std::pow(SIN_HALF, 2) - 1.0) / SIN_HALF;
			const F64 PYRAMID_HEIGHT   = std::pow(ICOSAHEDRON_SIDE, 2) / 2;
			const F64 RADIUS           = ICOSAHEDRON_SIDE / (2 * SIN_HALF);
			const F64 HEIGHT_ANGLE = std::atan((1.0f - PYRAMID_HEIGHT) / RADIUS);
				  
			const F64 A   = ICOSAHEDRON_SIDE;
			const F64 H   = PYRAMID_HEIGHT;
			const F64 R   = RADIUS;
			const F64 PHI = HEIGHT_ANGLE;

			const int N = PENTAGON_SIDES;

			//init icosahedron vertices
			Vec3f32 top    = Vec3f32(0.0f, +1.0f, 0.0f);
			Vec3f32 bottom = Vec3f32(0.0f, -1.0f, 0.0f);

			Vec3f32 topRing[N];
			Vec3f32 bottomRing[N];

			F64 angle{};
			for (int i = 0; i < N; i++)
			{
				angle = ANGLE * i;
				topRing[i] = Vec3f32(R * std::cos(angle), 1.0f - H, R * std::sin(angle));

				angle = ANGLE * i + HALF_ANGLE;
				bottomRing[i] = Vec3f32(R * std::cos(angle), H - 1.0f, R * std::sin(angle));
			}

			//assemble triangles
			std::vector<Vec3f32> triangles;
			for (int curr = 0; curr < N; curr++)
			{
				int prev = (curr + N - 1) % N;
				int next = (curr + N + 1) % N;

				//top + topRing[i] + topRing[i + 1]
				pushSplitedTriangles(triangles, top, topRing[next], topRing[curr], split);

				//bottom + bottomRing[i] + bottomRing[i + 1]
				pushSplitedTriangles(triangles, bottom, bottomRing[curr], bottomRing[next], split);

				//bottomRing[i] + topRing[i] + topRing[i + 1]
				pushSplitedTriangles(triangles, bottomRing[curr], topRing[curr], topRing[next], split);

				//topRing[i] + bottomRing[i] + bottomRing[i - 1]
				pushSplitedTriangles(triangles, topRing[curr], bottomRing[curr], bottomRing[prev], split);
			}
			return triangles;
		}
	}

	SphereMeshBuilder::SphereMeshBuilder(UInt32 subdivisions, const AttributeInfo& info)
		: m_subdivisions(subdivisions)
		, m_attributeInfo(info)
	{}

	[[nodiscard]]
	mesh::IMesh* SphereMeshBuilder::buildMesh()
	{
		auto vertices = sphere(m_subdivisions);

		SphereMesh::Data meshData;
		meshData.subdivisionCount = m_subdivisions;
		meshData.positions.createBuffer();
		meshData.positions.namedBufferData(
			  sizeof(Vec3f32) * vertices.size()
			, vertices.data()
			, gl::BufferUsage::StaticDraw
		);

		mesh::Mesh::VertexArray verticesArray;
		verticesArray.createArray();
		verticesArray.bind();
		verticesArray.setAttribPointerInBuffer(
			meshData.positions
			, gl::VertexArray::PointerInfo
			{
				  static_cast<GLuint>(m_attributeInfo.position)
				, gl::AttributeSize::Three
				, gl::DataType::Float
				, gl::GLBool::True
				, 0
				, nullptr
			}
		);
		verticesArray.unbind();

		mesh::Mesh::DrawInfo drawInfo;
		drawInfo.drawMode = gl::DrawMode::Triangles;
		drawInfo.first    = 0;
		drawInfo.elementsCount = vertices.size();

		return new SphereMesh(std::move(verticesArray), drawInfo, false, std::move(meshData));
	}
}