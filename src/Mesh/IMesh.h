#pragma once

// TODO : try add forward decl, I dunno if I can fwd inner class
#include "../glWrap/VertexArray/VertexArray.h"

namespace mesh
{
	class IMesh
	{
	public:
		using VertexArray = gl::VertexArray;
		using DrawInfo    = VertexArray::DrawInfo;


		virtual const VertexArray& getVertexArray() const = 0;

		virtual const DrawInfo& getDrawInfo() const = 0;

		virtual bool usingIndices() const = 0;
	};
}