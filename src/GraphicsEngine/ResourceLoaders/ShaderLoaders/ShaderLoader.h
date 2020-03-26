#pragma once

#include <glWrap/OpenGL.h>

namespace gl
{
	class Shader;
}

namespace graphics
{
	using gl::String;

	class SimpleShaderLoader
	{
	public:
		SimpleShaderLoader() = default;

		SimpleShaderLoader(const SimpleShaderLoader&) = default;
		SimpleShaderLoader(SimpleShaderLoader&&)      = default;

		~SimpleShaderLoader() = default;

		SimpleShaderLoader& operator = (const SimpleShaderLoader&) = default;
		SimpleShaderLoader& operator = (SimpleShaderLoader&&)      = default;


	public:
		gl::Shader shaderFromFile(gl::ShaderType type, const String& location, const String& attachment = "");
		gl::Shader shaderFromSource(gl::ShaderType type, const String& source, const String& shaderName, const String& attachment = "");


	private:
		String m_buffer;
	};
}
