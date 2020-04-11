#include "GraphicsEngine.h"

#include "ResourceLoaders/ShaderLoaders/ShaderLoader.h"
#include "ResourceLoaders/ShaderProgramBuilders/ShaderProgramBuilder.h"

#include <glWrap/Shader/Shader.h>
#include <glWrap/Shader/ShaderProgram.h>

namespace graphics
{
	const String SIMPLE_MAIN_VERT = 
		"out vec3 out" POSITION    ";\n" 
		"out vec3 out" TEXCOORD    ";\n"
		"out vec3 out" COLOR       ";\n"
		"out vec3 out" BARYCENTRIC ";\n"
		"out vec3 out" NORMAL      ";\n"
		"out vec3 out" TANGENT     ";\n"
		"out vec3 out" BITANGENT   ";\n"
		"void main()\n"
		"{\n"
		"    out" POSITION    "=" POSITION    ";\n"
		"    out" TEXCOORD    "=" TEXCOORD    ";\n"
		"    out" COLOR       "=" COLOR       ";\n"
		"    out" BARYCENTRIC "=" BARYCENTRIC ";\n"
		"    out" NORMAL      "=" NORMAL      ";\n"
		"    out" TANGENT     "=" TANGENT     ";\n"
		"    out" BITANGENT   "=" BITANGENT   ";\n"
		"    gl_Position = vec4(" POSITION ", 1.0);\n"
		"}\n";

	const String SIMPLE_MAIN_FRAG = 
		"in vec3 out" POSITION    ";\n" 
		"in vec3 out" TEXCOORD    ";\n"
		"in vec3 out" COLOR       ";\n"
		"in vec3 out" BARYCENTRIC ";\n"
		"in vec3 out" NORMAL      ";\n"
		"in vec3 out" TANGENT     ";\n"
		"in vec3 out" BITANGENT   ";\n"
		"out vec4 resColor;"
		"void main()\n"
		"{\n"
		"    resColor = vec4(" "out" POSITION "+" "out" TEXCOORD "+" "out" COLOR "+" "out" BARYCENTRIC "+" "out" NORMAL "+" "out" TANGENT "+" "out" BITANGENT ", 1.0);\n"
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

		String sourceVert = glslVersion + attribAttachment + SIMPLE_MAIN_VERT;
		String sourceFrag = glslVersion + SIMPLE_MAIN_FRAG;
		auto dummyVert = shaderLoader.shaderFromSource(gl::ShaderType::Vertex  , sourceVert, "DummyVert");
		auto dummyFrag = shaderLoader.shaderFromSource(gl::ShaderType::Fragment, sourceFrag, "DummyFrag");
		auto dummyProgram = programBuilder.buildProgram("Dummy", dummyVert, dummyFrag);

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
		auto e = glGetError();
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