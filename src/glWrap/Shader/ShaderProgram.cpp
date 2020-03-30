#include "ShaderProgram.h"

#include <glWrap/Shader/Shader.h>

namespace gl
{
	//statics
	thread_local String ShaderProgram::INFO_LOG;


	//constructors & destructor
	ShaderProgram::ShaderProgram()
	{}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) : Id(std::move(shaderProgram))
	{}


	ShaderProgram::~ShaderProgram()
	{
		deleteProgram();
	}


	//operators
	ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shaderProgram)
	{
		static_cast<Id&>(*this) = static_cast<Id&&>(shaderProgram);

		return *this;
	}


	//core functions
	void ShaderProgram::createProgram()
	{
		static_cast<Id&>(*this) = glCreateProgram();
	}

	void ShaderProgram::attachShader(const Shader& shader) const
	{
		glAttachShader(id(), shader.id());
	}

	void ShaderProgram::detachShader(const Shader& shader) const
	{
		glDetachShader(id(), shader.id());
	}


	void ShaderProgram::link() const
	{
		glLinkProgram(id());
	}

	void ShaderProgram::bind() const
	{
		glUseProgram(id());
	}

	void ShaderProgram::unbind() const
	{
		glUseProgram(Id::Empty);
	}


	//invoke compute shader(depends on openGL state, current program being in use)
	void ShaderProgram::dispatchCompute(const ComputeInfo& info) const
	{
		glDispatchCompute(
			  info.m_numGroupsX
			, info.m_numGroupsY
			, info.m_numGroupsZ
		);
	}


	//uniforms(depends on openGL state, current program being in use)
	void ShaderProgram::setUniformMat4f32(GLint location, const Mat4f32& mat) const
	{
		glUniformMatrix4fv(location, 1, static_cast<GLboolean>(GLBool::False), glm::value_ptr(mat));
	}

	void ShaderProgram::setUniformMat3f32(GLint location, const Mat3f32& mat) const
	{
		glUniformMatrix3fv(location, 1, static_cast<GLboolean>(GLBool::False), glm::value_ptr(mat));
	}

	void ShaderProgram::setUniformVec4f32(GLint location, const Vec4f32& vec) const
	{
		glUniform4fv(location, 1, glm::value_ptr(vec));
	}

	void ShaderProgram::setUniformVec3f32(GLint location, const Vec3f32& vec) const
	{
		glUniform3fv(location, 1, glm::value_ptr(vec));
	}

	void ShaderProgram::setUniformF32(GLint location, F32 value) const
	{
		glUniform1f(location, value);
	}


	void ShaderProgram::setUniformI32(GLint location, Int32 value) const
	{
		glUniform1i(location, value);
	}


	//locations
	GLint ShaderProgram::getUniformLocation(const String& name) const
	{
		return glGetUniformLocation(id(), name.c_str());
	}

	GLint ShaderProgram::getAttributeLocation(const String& name) const
	{
		return glGetAttribLocation(id(), name.c_str());
	}


	//delete & reset
	void ShaderProgram::deleteProgram()
	{
		glDeleteProgram(id());

		resetProgram();
	}

	void ShaderProgram::resetProgram()
	{
		resetId();
	}


	//checks
	bool ShaderProgram::linked() const
	{
		GLint result;

		glGetProgramiv(id(), GL_LINK_STATUS, &result);

		return result == GL_TRUE;
	}

	//info log
	const String& ShaderProgram::infoLog() const
	{
		String& infoLog = ShaderProgram::INFO_LOG;

		GLint length;
		GLint returned;

		glGetProgramiv(id(), GL_INFO_LOG_LENGTH, &length);
		infoLog.resize(length);

		glGetProgramInfoLog(id(), length, &returned, infoLog.data());
		infoLog.resize(returned);

		return infoLog;
	}
}
