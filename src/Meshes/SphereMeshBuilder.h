#pragma once

#include "../Mesh/IMeshBuilder.h"
#include "../GraphicsEngine/Attribs.h"

#include <Math/MathLib.h>

#include <vector>

namespace meshes
{
	using math::UInt32;

	class SphereMesh;

	class SphereMeshBuilder : public mesh::IMeshBuilder
	{
	public:
		struct AttributeInfo
		{
			graphics::Location position;
		};


	public:
		SphereMeshBuilder(UInt32 subdivisions, const AttributeInfo& info);

		SphereMeshBuilder(const SphereMeshBuilder&) = default;
		SphereMeshBuilder(SphereMeshBuilder&&)      = default;

		~SphereMeshBuilder() = default;

		SphereMeshBuilder& operator = (const SphereMeshBuilder&) = default;
		SphereMeshBuilder& operator = (SphereMeshBuilder&&)      = default;


	public:
		[[nodiscard]]
		virtual mesh::IMesh* buildMesh() override;


	private:
		UInt32 m_subdivisions;
		AttributeInfo m_attributeInfo;
	};
}