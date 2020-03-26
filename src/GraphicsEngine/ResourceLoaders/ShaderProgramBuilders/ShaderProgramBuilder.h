#pragma once

#include <iostream>
#include <glWrap/Shader/ShaderProgram.h>

namespace graphics
{
	using gl::String;

	class ShaderProgramBuilder
	{
	public:
		ShaderProgramBuilder() = default;

		ShaderProgramBuilder(const ShaderProgramBuilder&) = delete;
		ShaderProgramBuilder(ShaderProgramBuilder&&)      = delete;

		~ShaderProgramBuilder() = default;

		ShaderProgramBuilder& operator = (const ShaderProgramBuilder&) = delete;
		ShaderProgramBuilder& operator = (ShaderProgramBuilder&&)      = delete;


	public:
		template<class ... Shaders>
		gl::ShaderProgram buildProgram(const String& name, Shaders&& ... shaders)
		{
			gl::ShaderProgram program;

			program.createProgram();

			(program.attachShader(shaders), ...);

			program.link();
			if (program.linked())
			{
				std::cout << "Success. Program \"" << name << "\" built." << std::endl;
			}
			else
			{
				std::cout << "Failed to build program. \"" << name << "\"" << program.infoLog();
			}

			(program.detachShader(shaders), ...);

			return program;
		}
	};
}
