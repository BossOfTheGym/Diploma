#include "ShaderLoader.h"

#include <Common.h>

#include <glWrap/Shader/Shader.h>

namespace graphics
{
	gl::Shader SimpleShaderLoader::shaderFromFile(gl::ShaderType type, const String& location, const String& attachment)
	{
		alias::IFStream inputFile(location);
		m_buffer.clear();
		m_buffer.assign(
			  std::istreambuf_iterator<char>(inputFile)
			, std::istreambuf_iterator<char>()
		);

		return shaderFromSource(type, m_buffer, location, attachment);
	}

	gl::Shader SimpleShaderLoader::shaderFromSource(gl::ShaderType type, const String& source, const String& shaderName, const String& attachment)
	{
		gl::Shader shader;
		shader.createShader(type);
		shader.shaderSource(attachment + source);
		shader.compileShader();
		if (shader.compiled())
		{
			std::cout << "Compiled shader: " << shaderName << std::endl;
		}
		else
		{
			std::cout << "Failed to compile shader: " << shaderName << std::endl;
			std::cout << shader.infoLog() << std::endl;
		}

		return shader;
	}
}
