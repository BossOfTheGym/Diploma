#pragma once

#include <map>
#include <optional>

#include <glWrap/OpenGL.h>

#include "Attachment.h"


namespace graphics
{
	using gl::Float;

	class GraphicsEngine
	{
	public:
		using Attribs  = std::map<Name, Location>;

	public:
		GraphicsEngine(
			  const String& glslVersion      = "#version 400 core\n"
			, const String& attribAttachment = attrib_attachment<Float>::value
			, const String& typeAttachment   = type_attachment<Float>::value 
		);

		GraphicsEngine(const GraphicsEngine&) = delete;
		GraphicsEngine(GraphicsEngine&&) = delete;

		~GraphicsEngine() = default;

		GraphicsEngine& operator = (const GraphicsEngine&) = delete;
		GraphicsEngine& operator = (GraphicsEngine&&) = delete;


	public:
		std::optional<Location> getCommonAttribLocation(const Name& attrib);


		const String& getGlslVersion() const;

		const String& getAttribAttachment() const;

		const String& getTypeAttachment() const;
	private:
		Attribs m_baseAttribs;

		String m_version;
		String m_attribAttachment;
		String m_typeAttachment;
	};
}