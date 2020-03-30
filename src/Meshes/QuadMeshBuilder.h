#pragma once

#include "../Mesh/IMeshBuilder.h"
#include "../GraphicsEngine/Attribs.h"

namespace meshes
{
	class QuadMesh;

	class QuadMeshBuilder : mesh::IMeshBuilder
	{
	public:
		struct AttributeInfo
		{
			graphics::Location position;
		};


	public:
		QuadMeshBuilder(const AttributeInfo& info);

		QuadMeshBuilder(const QuadMeshBuilder&) = default;
		QuadMeshBuilder(QuadMeshBuilder&&)      = default;

		virtual ~QuadMeshBuilder() = default;

		QuadMeshBuilder& operator = (const QuadMeshBuilder&) = default;
		QuadMeshBuilder& operator = (QuadMeshBuilder&&)      = default;


	public:
		[[nodiscard]]
		virtual mesh::IMesh* buildMesh() override;


	private:
		AttributeInfo m_attributeInfo;
	};
}