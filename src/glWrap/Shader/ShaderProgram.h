#pragma once

#include <Math/MathLib.h>
#include <glWrap/ID/Id.h>


namespace gl
{
	class Shader;

	using namespace math;

	class ShaderProgram : public Id
	{
	private:
		static thread_local String INFO_LOG;


	public:
		struct ComputeInfo
		{
			GLuint m_numGroupsX = static_cast<GLuint>(0);
			GLuint m_numGroupsY = static_cast<GLuint>(0);
			GLuint m_numGroupsZ = static_cast<GLuint>(0);
		};


	public:
		ShaderProgram();

		ShaderProgram(ShaderProgram&& shaderProgram);


		~ShaderProgram();


		ShaderProgram& operator = (ShaderProgram&& shaderProgram);


	public:
		void createProgram();


		void attachShader(const Shader& shader) const;

		void detachShader(const Shader& shader) const;


		void link() const;

		void use() const;

		void unbind() const;


		void dispatchCompute(const ComputeInfo& info) const;


		void setUniformMat4f32(GLint location, const Mat4f32& mat) const;

		void setUniformMat3f32(GLint location, const Mat3f32& mat) const;

		void setUniformVec4f32(GLint location, const Vec4f32& vec) const;

		void setUniformVec3f32(GLint location, const Vec3f32& vec) const;

		void setUniformF32(GLint location, F32 value) const;


		void setUniformI32(GLint location, Int32 value) const;



		GLint getUniformLocation(const String& name) const;

		GLint getAttributeLocation(const String& name) const;


		void deleteProgram();


	public:
		bool linked() const;


		const String& infoLog() const;


	private:
		void resetProgram();


	private:
		ComputeInfo m_computeInfo;
	};
}
