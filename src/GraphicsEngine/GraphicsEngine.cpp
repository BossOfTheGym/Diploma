#include "GraphicsEngine.h"

#include "ResourceLoaders/ShaderLoaders/ShaderLoader.h"
#include "ResourceLoaders/ShaderProgramBuilders/ShaderProgramBuilder.h"

#include <glWrap/Shader/Shader.h>
#include <glWrap/Shader/ShaderProgram.h>

namespace graphics
{
	const String SIMPLE_MAIN = 
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(" POSITION ", 1.0);\n"
		"}\n";

	GraphicsEngine::GraphicsEngine(
		  const String& glslVersion
		, const String& attribAttachment
		, const String& typeAttachment   
	)
		: m_version(glslVersion)
		, m_attribAttachment(attribAttachment)
		, m_typeAttachment(typeAttachment)
		, m_baseAttribs()
	{
		ShaderProgramBuilder programBuilder;
		SimpleShaderLoader   shaderLoader;

		String source = glslVersion + attribAttachment + SIMPLE_MAIN;
		auto dummyShader  = shaderLoader.shaderFromSource(gl::ShaderType::Vertex, source, "Dummy");
		auto dummyProgram = programBuilder.buildProgram("Dummy", dummyShader);

		const char* attribs[] = 
		{
			  POSITION
			, TEXCOORD
			, COLOR
			, BARYCENTRIC
			, NORMAL
			, TANGENT
			, BITANGENT
		};
		for (auto attrib : attribs)
		{
			auto location = dummyProgram.getAttributeLocation(attrib);
			if (location != -1)
			{
				m_baseAttribs[attrib] = location;
			}
		}
	}

	std::optional<Location> GraphicsEngine::getCommonAttribLocation(const Name& attrib)
	{
		if (auto it = m_baseAttribs.find(attrib); it != m_baseAttribs.end())
		{
			return m_baseAttribs[attrib];
		}
		return std::nullopt;
	}

	const String& GraphicsEngine::getGlslVersion() const
	{
		return m_version;
	}

	const String& GraphicsEngine::getAttribAttachment() const
	{
		return m_attribAttachment;
	}
}