#pragma once

#include <Math/MathLib.h>
#include <glWrap/Buffer/Buffer.h>

#include "../Mesh/Mesh.h"


namespace meshes
{
	using math::UInt32;

	class SphereMesh : public mesh::Mesh
	{
	public:
		struct Data
		{
			UInt32 subdivisionCount{};

			gl::Buffer positions{};	
			gl::Buffer indices{};
		};


	public:
		SphereMesh(mesh::Mesh&& sphereArray, Data&& sphereData);

		SphereMesh(VertexArray&& vertexArray, const DrawInfo& info, bool usesIndices, Data&& sphreData);

		SphereMesh(const SphereMesh&) = delete;
		SphereMesh(SphereMesh&&);

		virtual ~SphereMesh() = default;

		SphereMesh& operator = (const SphereMesh&) = delete;
		SphereMesh& operator = (SphereMesh&&);


	public:
		const Data& getData() const;


	protected:
		Data m_sphereData;
	};
}