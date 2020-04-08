#pragma once

#include <Mesh/IMeshBuilder.h>
#include <GraphicsEngine/Attribs.h>


namespace meshes
{
	class BoxMesh;

	class BoxMeshBuilder : mesh::IMeshBuilder
	{
	public:
		struct AttributeInfo
		{
			graphics::Location position;
		};
		

	public:
		BoxMeshBuilder(const AttributeInfo& info);

		BoxMeshBuilder(const BoxMeshBuilder&) = default;
		BoxMeshBuilder(BoxMeshBuilder&&)      = default;

		virtual ~BoxMeshBuilder() = default;

		BoxMeshBuilder& operator = (const BoxMeshBuilder&) = default;
		BoxMeshBuilder& operator = (BoxMeshBuilder&&)      = default;


	public:
		[[nodiscard]]
		virtual mesh::IMesh* buildMesh() override;


	private:
		AttributeInfo m_attributeInfo;
	};
}