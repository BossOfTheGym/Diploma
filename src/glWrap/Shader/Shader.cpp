#include "Shader.h"

namespace gl
{
	//statics
	thread_local String Shader::INFO_LOG;


	//constructors
	Shader::Shader()
		: Id()
		, m_type(ShaderType::None)
	{}

	Shader::Shader(Shader&& shader) : Id()
	{
		*this = std::move(shader);
	}


	//destructor
	Shader::~Shader()
	{
		deleteShader();
	}

	//operators
	Shader& Shader::operator = (Shader&& shader)
	{
		static_cast<Id&>(*this) = static_cast<Id&&>(shader);

		std::swap(m_type, shader.m_type);

		return *this;
	}


	//core functions
	void Shader::createShader(ShaderType type)
	{
		static_cast<Id&>(*this) = glCreateShader(static_cast<GLenum>(type));

		m_type = type;
	}

	void Shader::shaderSource(const String& source)
	{
		const GLchar* src = source.c_str();
		GLint size        = static_cast<GLint>(source.size());

		glShaderSource(id(), 1, &src, &size);
	}

	void Shader::compileShader()
	{
		glCompileShader(id());
	}

	void Shader::deleteShader()
	{
		glDeleteShader(id());

		resetShader();
	}

	void Shader::resetShader()
	{
		resetId();

		m_type = ShaderType::None;
	}


	//checks
	bool Shader::compiled() const
	{
		if (!valid())
		{
			return false;
		}

		GLint result;
		glGetShaderiv(id(), GL_COMPILE_STATUS, &result);

		return result == GL_TRUE;
	}

	const String& Shader::infoLog() const
	{
		String& infoLog = Shader::INFO_LOG;

		GLint length;
		GLint returned;

		glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &length);
		infoLog.resize(length);

		glGetShaderInfoLog(id(), length, &returned, infoLog.data());
		infoLog.resize(returned);

		return infoLog;
	}

	//get & set
	ShaderType Shader::type() const
	{
		return m_type;
	}
}